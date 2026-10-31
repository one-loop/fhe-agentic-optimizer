# Invalid timings: do not use

This re-scoring ran 2026-10-27 02:21-11:53 with the Mac's lid closed. `pmset -g log`
shows clamshell sleep at 02:29 and about 40 sleep events during the session. Timing
spread was about +-10% instead of the usual +-0.1%. The search itself finished before
the lid closed. `../phase2_static_feedback/` holds the clean re-measurement of the same
final kernels. Kept only as a record.
