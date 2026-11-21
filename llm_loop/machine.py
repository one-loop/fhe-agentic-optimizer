"""State of the machine during timing: power source and lid (macOS).

Timings taken on battery or with the lid closed are throttled and noisier; one result set had to be
discarded for that (`results/phase2_static_feedback_INVALID_lid_closed`). Scripts that time kernels call
`require_stable_power()` before starting and record `machine_state()` in their metadata.
"""
import platform
import subprocess
import sys
from typing import Dict, Optional


def _run(cmd) -> str:
    try:
        return subprocess.run(cmd, capture_output=True, text=True, timeout=10).stdout
    except (OSError, subprocess.SubprocessError):
        return ""


def power_source() -> Optional[str]:
    """'AC Power', 'Battery Power', or None when unknown (not macOS)."""
    if platform.system() != "Darwin":
        return None
    out = _run(["pmset", "-g", "batt"])
    if "'AC Power'" in out:
        return "AC Power"
    if "'Battery Power'" in out:
        return "Battery Power"
    return None


def lid_closed() -> Optional[bool]:
    if platform.system() != "Darwin":
        return None
    out = _run(["ioreg", "-r", "-k", "AppleClamshellState", "-d", "4"])
    if '"AppleClamshellState" = Yes' in out:
        return True
    if '"AppleClamshellState" = No' in out:
        return False
    return None


def machine_state() -> Dict[str, object]:
    return {"power_source": power_source(), "lid_closed": lid_closed()}


def require_stable_power(allow_unstable: bool = False) -> Dict[str, object]:
    """Exit unless the machine is on AC power with the lid open (or the state is unknown)."""
    state = machine_state()
    problems = []
    if state["power_source"] == "Battery Power":
        problems.append("the machine is on battery")
    if state["lid_closed"] is True:
        problems.append("the lid is closed")
    if problems:
        message = f"timing would be unreliable: {' and '.join(problems)}. Plug in and open the lid"
        if not allow_unstable:
            sys.exit(f"{message}, or pass --allow-unstable-power to time anyway (the results record it).")
        print(f"WARNING: {message}. Continuing because --allow-unstable-power was given.", file=sys.stderr)
    return state
