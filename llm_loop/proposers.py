"""Proposers: where candidate kernels come from.

ClaudeCodeProposer  Claude through Claude Code's headless mode (`claude -p`),
                    signed in with a Claude subscription: no API key, no
                    per-token billing (usage counts against the plan's limits).
                    ANTHROPIC_API_KEY / ANTHROPIC_AUTH_TOKEN are removed from
                    the call's environment, because in headless mode a key, when
                    present, is always used and billed.
ReplayProposer      replays answers from files (answer_<i>.md or attempt_<i>.cpp)
                    to test the loop offline, without any model call.

Every call is stateless: the loop sends the whole context each time.
"""
import json
import os
import shutil
import subprocess
import tempfile
import time
from dataclasses import dataclass, field
from pathlib import Path
from typing import Optional


@dataclass
class Reply:
    text: str
    seconds: float = 0.0
    usage: dict = field(default_factory=dict)
    cost_usd: Optional[float] = None  # client-side estimate; not billed on a subscription
    error: Optional[str] = None


class UsageLimitReached(RuntimeError):
    """The subscription's session or weekly limit is reached: stop and resume later."""


class ClaudeCodeProposer:
    name = "claude-code"

    def __init__(self, model: str = "claude-haiku-5-5", effort: str = "medium", timeout: float = 900,
                 executable: str = "claude"):
        self.model, self.effort, self.timeout = model, effort, timeout
        self.executable = shutil.which(executable) or executable
        self.workdir = Path(tempfile.mkdtemp(prefix="llm_loop_claude_"))  # empty: no project files to load

    def describe(self) -> dict:
        version = subprocess.run([self.executable, "--version"], capture_output=True, text=True).stdout.strip()
        return {"proposer": self.name, "model": self.model, "effort": self.effort, "claude_code": version}

    def propose(self, system: str, prompt: str) -> Reply:
        env = {k: v for k, v in os.environ.items() if k not in ("ANTHROPIC_API_KEY", "ANTHROPIC_AUTH_TOKEN")}
        cmd = [self.executable, "-p", "--model", self.model, "--effort", self.effort,
               "--system-prompt", system, "--tools", "", "--safe-mode", "--no-session-persistence",
               "--output-format", "json"]
        start = time.time()
        out = subprocess.run(cmd, input=prompt, capture_output=True, text=True, timeout=self.timeout,
                             cwd=self.workdir, env=env)
        seconds = time.time() - start
        try:
            data = json.loads(out.stdout)
        except json.JSONDecodeError:
            return Reply("", seconds, error=f"claude exited {out.returncode}: {(out.stderr or out.stdout)[-1000:]}")
        text = data.get("result") or ""
        usage = dict(data.get("usage", {}))
        usage["models"] = sorted(data.get("modelUsage", {}))  # the model(s) that actually served the call
        if data.get("is_error") or out.returncode != 0:
            if "limit" in text.lower():
                raise UsageLimitReached(text)
            return Reply(text, seconds, usage, data.get("total_cost_usd"), error=text or "error")
        return Reply(text, seconds, usage, data.get("total_cost_usd"))


class ReplayProposer:
    """Replays <dir>/answer_<i>.md (a full answer) or <dir>/attempt_<i>.cpp (a kernel), i = 1, 2, ..."""

    name = "replay"

    def __init__(self, directory: Path):
        self.directory = Path(directory)
        self.calls = 0

    def describe(self) -> dict:
        return {"proposer": self.name, "directory": str(self.directory)}

    def propose(self, system: str, prompt: str) -> Reply:
        self.calls += 1
        for name in (f"answer_{self.calls}.md", f"attempt_{self.calls}.cpp"):
            path = self.directory / name
            if path.exists():
                text = path.read_text()
                return Reply(text if name.endswith(".md") else f"Replayed {name}.\n```cpp\n{text}```")
        return Reply("(no more replay files)")
