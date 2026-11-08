I can't find a cheaper circuit, and I should correct my last reply. It described a "probe" kernel and then submitted the baseline, which was confusing. All three attempts have been the same circuit.

One ciphertext-ciphertext multiply with its relinearization and rescale is the floor for `y = x0 * x1`:

- **Square identities:** they need at least two squares and a constant 1/2 or 1/4. Constants can't be used in CKKS here, and the extra plaintext multiply would cost another level.
- **Rotations and plaintext arithmetic:** they only add operations.
- **Relinearization and rescale:** CHEHAB inserts them itself, so I can't remove them.

Any circuit that differs from the baseline therefore has more operations, so it can't win the timing comparison. I'm submitting the baseline kernel again as the final answer and won't use the remaining attempts on circuits I expect to be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```