I don't have a cheaper circuit to offer. `y = x*A + B` needs the per-slot scale applied, which takes one plaintext multiply and its rescale. It also needs the shift added, which takes one plaintext add. Both are already in the baseline, and neither can be folded away without plaintext-plaintext arithmetic, which CHEHAB rejects. The two variants I tried were either rejected (attempt 2) or slower (attempt 3, 3.95 ms against 3.77 ms), because they added operations.

I'm returning the baseline form unchanged. It will count as identical to the baseline, so it can't become a new best.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```