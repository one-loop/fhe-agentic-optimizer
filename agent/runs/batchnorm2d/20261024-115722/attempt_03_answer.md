The kernel is already minimal: one plaintext multiply with rescale and one plaintext add. The differences between runs are measurement noise, not real gains. I can't remove an operation without changing the function, since B has to be encoded and added, and A has to be encoded and multiplied.

One variant is worth trying: `(x * a) + b` is what CHEHAB already emits, so reordering gives nothing. Adding `b` before the multiply would need B/A, which is plaintext-plaintext arithmetic and not allowed. I'm keeping the best form.

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