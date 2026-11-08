The baseline is already close to minimal: one plaintext multiply with its rescale, and one plaintext add. I don't see a way to drop the multiply or the level, because the per-slot scale `A` has to be applied and plaintext-plaintext arithmetic is not allowed. So I'm keeping the same structure and accumulating the shift in place. CHEHAB will probably normalize this to the same circuit, so it may not count as a new best.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```