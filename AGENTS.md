# Agent Entry Point

All coding agents should read these files in order:

1. [`CLAUDE.md`](CLAUDE.md) for constraints and the current objective.
2. [`docs/agent_handoff.md`](docs/agent_handoff.md) for the exact implementation, environment, evidence, commands, and next work.
3. [`agent/README.md`](agent/README.md) for the feedback loop's maintained usage documentation.
4. [`docs/replication.md`](docs/replication.md) when setting the loop up on a new machine.

Preserve run evidence and any uncommitted work, never expose the ignored `.env`, and ask before running live model calls (subscription usage) or committing/pushing changes.

