"""Static analysis of the SEAL CKKS code CHEHAB generates (he/_gen_he_fhe.cpp).

Counts evaluator operations the way run_benchmarks.py does (so the numbers are
comparable with the Phase 1 records), separates the scale-alignment multiplies
CHEHAB inserts (a multiply_plain by an encoded 1.0, plus its rescale), and
tracks every ciphertext's level through the program to give the levels each
output consumes and an annotated trace of fhe().
"""
import re
from dataclasses import dataclass, field
from typing import Dict, List, Tuple

# Same mapping as run_benchmarks.py's OP_FIELDS.
OP_FIELDS = {
    "add": "op_add", "add_plain": "op_add_plain", "sub": "op_sub", "sub_plain": "op_sub_plain",
    "multiply": "op_multiply", "multiply_plain": "op_multiply_plain", "square": "op_square",
    "relinearize": "op_relinearize", "rescale_to_next": "op_rescale", "mod_switch_to_next": "op_mod_switch",
    "mod_switch_to": "op_mod_switch", "rotate_rows": "op_rotate", "rotate_columns": "op_rotate",
    "rotate_vector": "op_rotate", "negate": "op_negate",
}
LEVEL_OPS = {"rescale_to_next", "mod_switch_to_next"}
CIPHER_BINARY = {"add", "sub", "multiply"}

_CALL = re.compile(r"^\s*evaluator\.(\w+)\((.*)\);\s*$")
_INPUT = re.compile(r'^\s*Ciphertext (\w+) = encrypted_inputs\.at\("([^"]+)"\);')
_OUTPUT = re.compile(r'encrypted_outputs\.emplace\("([^"]+)", move\((\w+)\)\)')
_ALIGN_PLAIN = re.compile(r"encoder\.encode\(1\.0, .*?, (\w+)\);")


@dataclass
class StaticInfo:
    ops: Dict[str, int] = field(default_factory=dict)  # run_benchmarks.py op_* fields
    align_multiplies: int = 0  # multiply_plain by 1.0 inserted for scale alignment
    output_levels: Dict[str, int] = field(default_factory=dict)  # levels consumed per output
    trace: List[Tuple[str, int]] = field(default_factory=list)  # (statement, level after it)
    unknown: List[str] = field(default_factory=list)  # evaluator calls this module does not model

    @property
    def levels(self) -> int:
        return max(self.output_levels.values()) if self.output_levels else 0

    @property
    def key_switches(self) -> int:
        """Relinearizations plus rotations: the dominant CKKS costs."""
        return self.ops.get("op_relinearize", 0) + self.ops.get("op_rotate", 0)

    @property
    def multiply_plain_useful(self) -> int:
        return self.ops.get("op_multiply_plain", 0) - self.align_multiplies

    def summary(self) -> Dict[str, int]:
        return {
            "levels": self.levels, "ct_ct_mul": self.ops.get("op_multiply", 0) + self.ops.get("op_square", 0),
            "ct_pt_mul": self.multiply_plain_useful, "align_mul": self.align_multiplies,
            "rotate": self.ops.get("op_rotate", 0), "mod_switch": self.ops.get("op_mod_switch", 0),
            "rescale": self.ops.get("op_rescale", 0), "add": self.ops.get("op_add", 0) + self.ops.get("op_sub", 0),
            "add_plain": self.ops.get("op_add_plain", 0) + self.ops.get("op_sub_plain", 0),
        }


def fhe_body(source: str) -> List[str]:
    """Lines of the generated fhe() function body."""
    start = source.find("void fhe(")
    if start < 0:
        raise ValueError("no fhe() in the generated source")
    body_start = source.find("{", source.find(")\n{", start)) + 1
    depth, i = 1, body_start
    while depth and i < len(source):
        depth += {"{": 1, "}": -1}.get(source[i], 0)
        i += 1
    return source[body_start:i - 1].strip("\n").split("\n")


def analyze(source: str) -> StaticInfo:
    info = StaticInfo(ops={f: 0 for f in set(OP_FIELDS.values())})
    level: Dict[str, int] = {}
    align_plains = set()
    for line in fhe_body(source):
        stmt = line.strip()
        if m := _INPUT.match(stmt):
            level[m.group(1)] = 0
            info.trace.append((stmt, 0))
            continue
        if m := _ALIGN_PLAIN.search(stmt):
            align_plains.add(m.group(1))
            continue
        if m := _OUTPUT.search(stmt):
            info.output_levels[m.group(1)] = level.get(m.group(2), 0)
            info.trace.append((stmt, level.get(m.group(2), 0)))
            continue
        m = _CALL.match(stmt)
        if not m:
            continue
        method, args = m.group(1), [a.strip() for a in m.group(2).split(",")]
        if method in OP_FIELDS:
            info.ops[OP_FIELDS[method]] += 1
        else:
            info.unknown.append(stmt)
        src, dst = args[0], args[-1]
        if method in LEVEL_OPS:
            level[dst] = level.get(src, 0) + 1
        elif method in CIPHER_BINARY:
            level[dst] = max(level.get(src, 0), level.get(args[1], 0))
        else:  # *_plain, relinearize, rotate_vector, negate, square: level of the ciphertext operand
            level[dst] = level.get(src, 0)
        if method == "multiply_plain" and args[1] in align_plains:
            info.align_multiplies += 1
            stmt += "  // scale alignment"
        info.trace.append((stmt, level[dst]))
    return info


def annotated_trace(info: StaticInfo) -> str:
    """fhe() with the level after each statement, for feedback on level/scale problems."""
    return "\n".join(f"[L{lvl}] {stmt}" for stmt, lvl in info.trace)
