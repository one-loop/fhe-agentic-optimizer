The baseline already uses the minimum I can see: one plaintext multiply with its rescale, and one plaintext add. This attempt rewrites it as `y = b - (-x)*a`. That adds a ciphertext negation, so it is probably not faster, but it is a different circuit and tests whether the plaintext subtract is handled any more cheaply than the add.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
```