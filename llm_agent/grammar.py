"""VecLang S-expression grammar parser, AST validator, and slot extractor.

Validates that LLM candidate expressions follow correct VecLang syntax and semantics
before sending them to the CHEHAB compiler.
"""

from typing import List, Tuple, Optional, Any, Set
import re


# Operator categories
SCALAR_OPS = {"+", "-", "*", "Neg", "Add", "Minus", "Mul"}
VECTOR_BINARY_OPS = {"VecAdd", "VecMinus", "VecMul"}
VECTOR_UNARY_OPS = {"VecNeg"}
VECTOR_ROT_OPS = {"VecAddRot", "VecMulRot", "VecMinusRot", "<<"}
ALL_KNOWN_OPS = (
    SCALAR_OPS
    | VECTOR_BINARY_OPS
    | VECTOR_UNARY_OPS
    | VECTOR_ROT_OPS
    | {"Vec"}
)


class ASTNode:
    """Represents a node in the VecLang S-expression tree."""

    def __init__(self, op_or_val: str, args: Optional[List["ASTNode"]] = None, is_leaf: bool = False):
        self.val = op_or_val
        self.args: List[ASTNode] = args if args is not None else []
        self.is_leaf = is_leaf

    @property
    def is_op(self) -> bool:
        return not self.is_leaf

    def __repr__(self) -> str:
        if self.is_leaf:
            return self.val
        return f"({self.val} {' '.join(repr(a) for a in self.args)})"

    def to_sexpr(self) -> str:
        if self.is_leaf:
            return self.val
        return f"({self.val} {' '.join(a.to_sexpr() for a in self.args)})"


def tokenize(s: str) -> List[str]:
    """Tokenize an S-expression string."""
    return s.replace("(", " ( ").replace(")", " ) ").split()


def parse_tokens(tokens: List[str]) -> Tuple[ASTNode, List[str]]:
    """Recursively parse tokens into an ASTNode."""
    if not tokens:
        raise SyntaxError("Unexpected end of expression while parsing.")

    token = tokens.pop(0)
    if token == "(":
        if not tokens:
            raise SyntaxError("Unexpected end of expression after '('.")
        op = tokens.pop(0)
        args: List[ASTNode] = []

        while tokens and tokens[0] != ")":
            arg, tokens = parse_tokens(tokens)
            args.append(arg)

        if not tokens:
            raise SyntaxError(f"Missing closing parenthesis for operator '({op}'.")
        tokens.pop(0)  # Consume ')'
        return ASTNode(op, args, is_leaf=False), tokens
    elif token == ")":
        raise SyntaxError("Unexpected closing parenthesis ')' with no matching open parenthesis.")
    else:
        # Leaf: variable or constant literal
        return ASTNode(token, [], is_leaf=True), tokens


def parse_sexpr(s: str) -> ASTNode:
    """Parse an S-expression string into an ASTNode."""
    tokens = tokenize(s)
    if not tokens:
        raise SyntaxError("Empty S-expression string.")
    ast, remaining = parse_tokens(tokens)
    if remaining:
        raise SyntaxError(f"Unexpected trailing tokens: {' '.join(remaining)}")
    return ast


def extract_variables(node: ASTNode) -> Set[str]:
    """Find all variable names referenced in the AST."""
    vars_found: Set[str] = set()
    if node.is_leaf:
        if not node.val.isdigit() and not (node.val.startswith(("-", "+")) and node.val[1:].isdigit()):
            vars_found.add(node.val)
    else:
        for arg in node.args:
            vars_found |= extract_variables(arg)
    return vars_found


def extract_vec_lengths(node: ASTNode) -> List[int]:
    """Find lengths of all (Vec ...) constructors."""
    lengths: List[int] = []
    if not node.is_leaf:
        if node.val == "Vec":
            lengths.append(len(node.args))
        for arg in node.args:
            lengths.extend(extract_vec_lengths(arg))
    return lengths


def validate_expression(
    expr_str: str,
    available_vars: Optional[Set[str]] = None,
    expected_slot_count: Optional[int] = None,
) -> Tuple[bool, str, Optional[ASTNode]]:
    """Statically validate the VecLang S-expression.

    Returns:
        (is_valid, message, parsed_ast)
    """
    if not expr_str or not expr_str.strip():
        return False, "Empty expression provided.", None

    # Check balanced parentheses count first
    open_count = expr_str.count("(")
    close_count = expr_str.count(")")
    if open_count != close_count:
        return (
            False,
            f"Unbalanced parentheses: found {open_count} '(' and {close_count} ')'.",
            None,
        )

    # Attempt to parse
    try:
        ast = parse_sexpr(expr_str)
    except SyntaxError as e:
        return False, f"Syntax Error: {str(e)}", None
    except Exception as e:
        return False, f"Parsing Error: {str(e)}", None

    # Semantic checks on AST
    def check_node(node: ASTNode) -> Optional[str]:
        if node.is_leaf:
            return None

        op = node.val
        if op not in ALL_KNOWN_OPS:
            return f"Unknown operator '{op}'. Valid operators are: {sorted(list(ALL_KNOWN_OPS))}"

        # Check arities
        if op in VECTOR_ROT_OPS:
            if len(node.args) != 2:
                return f"Operator '{op}' requires exactly 2 arguments (vector and shift), got {len(node.args)}."
            shift_node = node.args[1]
            if not shift_node.is_leaf or not shift_node.val.isdigit():
                return f"Second argument of '{op}' must be an integer shift constant, got '{shift_node.val}'."

        elif op in VECTOR_BINARY_OPS:
            if len(node.args) < 2:
                return f"Operator '{op}' requires at least 2 arguments, got {len(node.args)}."

        elif op in VECTOR_UNARY_OPS:
            if len(node.args) != 1:
                return f"Operator '{op}' requires exactly 1 argument, got {len(node.args)}."

        elif op == "Vec":
            if len(node.args) == 0:
                return "Operator 'Vec' cannot be empty."

        for arg in node.args:
            err = check_node(arg)
            if err:
                return err
        return None

    err = check_node(ast)
    if err:
        return False, err, ast

    # Check vector packing consistency
    vec_lengths = extract_vec_lengths(ast)
    if vec_lengths:
        first_len = vec_lengths[0]
        for l in vec_lengths:
            if l != first_len:
                return (
                    False,
                    f"Inconsistent vector dimensions: found (Vec ...) of lengths {vec_lengths}. All packed vectors must have uniform length.",
                    ast,
                )

        if expected_slot_count is not None and first_len != expected_slot_count:
            return (
                False,
                f"Vector length {first_len} does not match expected slot_count {expected_slot_count}.",
                ast,
            )

    # Check variable coverage if provided
    if available_vars:
        used_vars = extract_variables(ast)
        invalid_vars = used_vars - available_vars
        if invalid_vars:
            return (
                False,
                f"Unknown variable(s) used in expression: {sorted(list(invalid_vars))}. Available variables are: {sorted(list(available_vars))}",
                ast,
            )

    return True, "Valid S-expression.", ast


def extract_code_from_response(response_text: str) -> Optional[str]:
    """Robustly extract the S-expression from the LLM output.

    Looks for:
    1. Explicit 'PROPOSED_EXPRESSION:' label
    2. Markdown code fences ```lisp / ```clojure / ```scheme / ```
    3. Balanced top-level S-expression (Vec ...), (VecAdd ...), etc.
    """
    if not response_text:
        return None

    # 1. First balanced S-expression after the last PROPOSED_EXPRESSION: label
    #    (it may span several lines and sit inside a code fence).
    labels = list(re.finditer(r"PROPOSED_EXPRESSION\s*:", response_text, re.IGNORECASE))
    if labels:
        rest = response_text[labels[-1].end():]
        start = rest.find("(")
        if start != -1:
            depth = 0
            for idx in range(start, len(rest)):
                if rest[idx] == "(":
                    depth += 1
                elif rest[idx] == ")":
                    depth -= 1
                    if depth == 0:
                        return " ".join(rest[start : idx + 1].split())
            # Unbalanced: return it as written so validation reports the real
            # problem, instead of falling back to a fragment from the prose.
            body = rest[start:].split("```")[0]
            return " ".join(body.split())
        return None

    # 2. Search for code blocks
    code_blocks = re.findall(r"```(?:lisp|scheme|clojure|sexpr|text)?\s*([\s\S]*?)```", response_text, re.IGNORECASE)
    for block in code_blocks:
        block = block.strip()
        if block.startswith("(") and block.endswith(")"):
            if block.count("(") == block.count(")"):
                return block
        # Maybe inside the block there is a parenthesized expression
        m = re.search(r"(\([A-Za-z0-9_+\-*\s<>()]+\))", block)
        if m and m.group(1).count("(") == m.group(1).count(")"):
            return m.group(1).strip()

    # 3. Direct regex match on any top-level balanced S-expression starting with '(' and a known op
    pattern = r"(\((?:Vec|VecAdd|VecMul|VecMinus|VecAddRot|VecMulRot|\+|\*)\s+[\s\S]*?\))"
    for match in re.finditer(pattern, response_text):
        cand = match.group(1).strip()
        # Ensure balanced
        open_c = 0
        end_idx = -1
        for idx, ch in enumerate(cand):
            if ch == "(":
                open_c += 1
            elif ch == ")":
                open_c -= 1
                if open_c == 0:
                    end_idx = idx + 1
                    break
        if end_idx != -1:
            clean_cand = cand[:end_idx].strip()
            if clean_cand.count("(") == clean_cand.count(")"):
                return clean_cand

    return None
