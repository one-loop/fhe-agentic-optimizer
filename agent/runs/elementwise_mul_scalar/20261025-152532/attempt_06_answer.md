I have no cheaper circuit to offer. The baseline's single plaintext multiply with its rescale is already the minimum for `x0 * s`. Every variant I can construct adds operations, so none can beat it. I'm keeping the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```