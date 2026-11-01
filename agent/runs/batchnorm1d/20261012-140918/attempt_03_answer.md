I'm stopping here. Three attempts have produced the same circuit, and the timings (9.59, 9.66, 9.77 ms) differ only by measurement noise.

The circuit is already minimal for this function:
- **One ciphertext-plaintext multiply** by `A`. This is required, since `A` is a per-slot scale known only at runtime. It is also the cheapest multiply type (no relinearization).
- **One plaintext add** of `B`. This is required, since it is the shift term.
- **No rotations, no ciphertext-ciphertext multiplies, no extra levels.**

I considered alternatives and none is valid or faster:
- Rewriting as `(x + B/A) * A` needs a division, which CKKS does not support.
- Reordering the add and multiply keeps the same operation count.
- Pre-scaling `B` or folding constants needs numeric constants, which are forbidden, or extra operations.

I'm keeping the baseline as the submitted kernel. Further attempts would only re-measure the same two operations.

```cpp
// y = x * A + B: one plaintext multiply (depth 1) and one plaintext add.
// Already minimal for this function; no rotations or ciphertext multiplies.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```