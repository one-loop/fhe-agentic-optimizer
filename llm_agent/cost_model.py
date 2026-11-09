"""Static analytical cost and multiplicative depth evaluator for FHE IR expressions.

Mirrors CHEHAB's cost model (egraphs/src/cost.rs and RL/pytrs/cost.py) to provide
instant local feedback and candidate ranking.
"""

from typing import Tuple, Dict, Any
from .grammar import ASTNode, parse_sexpr, validate_expression


LITERAL_COST = 0
SCALAR_OP_COST = 250
VEC_OP_COST = 1
VEC_MUL_COST = 100
VEC_ROT_COST = 1051


def operations_cost(node: ASTNode) -> int:
    """Compute base operation cost of an AST node."""
    if node.is_leaf:
        return LITERAL_COST

    op = node.val
    cost = 0

    if op in ("+", "Add", "-", "Minus", "*", "Mul", "Neg"):
        cost = SCALAR_OP_COST
    elif op in ("VecAdd", "VecMinus", "VecNeg"):
        cost = VEC_OP_COST
    elif op == "VecMul":
        cost = VEC_MUL_COST
    elif op in ("VecAddRot", "VecMinusRot"):
        cost = VEC_ROT_COST
    elif op == "VecMulRot":
        cost = VEC_ROT_COST + VEC_MUL_COST
    elif op == "<<":
        cost = 50
    elif op == "Vec":
        cost = 0
    else:
        cost = 10

    for arg in node.args:
        cost += operations_cost(arg)

    return cost


def get_multiplicative_depth(node: ASTNode) -> int:
    """Recursively compute the multiplicative depth of an expression."""
    if node.is_leaf or not node.args:
        return 0

    op = node.val
    if op in ("*", "Mul", "VecMul", "VecMulRot"):
        return 1 + max((get_multiplicative_depth(arg) for arg in node.args), default=0)
    else:
        return max((get_multiplicative_depth(arg) for arg in node.args), default=0)


def count_rotations(node: ASTNode) -> int:
    """Count the total number of rotation operations in the expression."""
    if node.is_leaf:
        return 0

    op = node.val
    count = 1 if op in ("VecAddRot", "VecMulRot", "VecMinusRot", "<<") else 0
    for arg in node.args:
        count += count_rotations(arg)
    return count


def count_multiplications(node: ASTNode) -> int:
    """Count the total number of multiplications in the expression."""
    if node.is_leaf:
        return 0

    op = node.val
    count = 1 if op in ("*", "Mul", "VecMul", "VecMulRot") else 0
    for arg in node.args:
        count += count_multiplications(arg)
    return count


def evaluate_cost_metrics(expr_or_ast) -> Dict[str, Any]:
    """Compute all static FHE metrics for an expression or AST."""
    if isinstance(expr_or_ast, str):
        valid, msg, ast = validate_expression(expr_or_ast)
        if not valid or ast is None:
            return {
                "valid": False,
                "error": msg,
                "cost": float("inf"),
                "multiplicative_depth": float("inf"),
                "rotations": float("inf"),
                "multiplications": float("inf"),
            }
    else:
        ast = expr_or_ast

    ops_cost = operations_cost(ast)
    mul_depth = get_multiplicative_depth(ast)
    rots = count_rotations(ast)
    muls = count_multiplications(ast)

    # Composite cost
    total_cost = ops_cost + (rots * 1000) + (mul_depth * 10)

    return {
        "valid": True,
        "cost": total_cost,
        "operations_cost": ops_cost,
        "multiplicative_depth": mul_depth,
        "rotations": rots,
        "multiplications": muls,
    }
