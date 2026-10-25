I'm stopping here. The kernel is the same every time, and the measured times drift upward (17.74 → 22.04 ms) without any code change, which points to machine load rather than the kernel. The operation count can't go lower: one ciphertext-plaintext multiply for `x*A` and one plaintext add for `+B`. A and B are per-slot vectors, so no scalar shortcut exists, and the rules forbid plaintext-plaintext arithmetic and numeric constants.

The final kernel is the baseline, which measured 17.74 ms, the best result:

```cpp
// y = x * A + B: one ciphertext-plaintext multiply and one plaintext add.
// This is the minimal evaluation; no rotations or ciphertext multiplications.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```