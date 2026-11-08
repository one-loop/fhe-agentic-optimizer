I have no cheaper circuit for the last attempt either. `x0 * s` is already one plaintext multiply with one rescale, and I can't remove either without losing the scaling by `s`. The baseline stays the best result at 8.81 ms. I'm submitting it unchanged, and it will be reported as identical again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```