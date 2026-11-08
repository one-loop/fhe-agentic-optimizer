I'm stopping here. The circuit is already at its minimum, and every new kernel I send is a duplicate of attempt 1, so it can't be confirmed as an improvement.

- **Why 27 adds is the floor:** Each of the 9 outputs sums 4 distinct inputs, which takes 3 additions. The patches are disjoint, so no partial sum can be shared between outputs.
- **Why packing doesn't help:** The inputs are single-slot ciphertexts, so rotations do nothing. Combining values would need constant or plaintext multiplies (about 1 ms each). That costs more than the 0.3 ms adds it would replace.
- **What measured:** The balanced tree (depth 2) timed at 7.33 ms against 7.56 ms for the baseline in single timings. In the alternating-batch comparison it was only about 2% faster (7.50 vs 7.47 ms), below the 3% threshold.

I'm leaving the remaining 2 attempts unused. The baseline stays the best confirmed kernel, and the balanced-tree kernel in attempt 1 is the only variant I can offer.