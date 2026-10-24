Add the linear term to z^2 directly.
```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Ciphertext z = x * alpha;
  Ciphertext y = z * c1 + z * z;
  y.set_output("y");
}
```
