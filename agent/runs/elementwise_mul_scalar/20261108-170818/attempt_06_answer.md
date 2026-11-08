No cheaper circuit exists that I can find, so this is the baseline kernel again. It is one ciphertext-plaintext multiplication plus its rescale, and any other form adds operations.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```