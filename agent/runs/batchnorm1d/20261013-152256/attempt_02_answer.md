I don't expect a faster kernel from further changes. This attempt measured the same 17.74 ms as the baseline, and the computation has no redundant work left to remove. `y = x*A + B` needs one ciphertext-plaintext multiply and one plaintext add. Folding B into the multiply (for example as `(x + B/A)*A`) would need a division, and plaintext-plaintext arithmetic is disallowed. Rotations and ciphertext multiplications aren't needed at all.

I'm keeping the same kernel as the best result so far.

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (one level) followed by
// one plaintext-vector add. This is the minimal evaluation of the task.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
```