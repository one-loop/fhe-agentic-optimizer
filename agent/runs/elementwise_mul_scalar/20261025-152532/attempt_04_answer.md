I have nothing new to try. The only circuits I can construct that differ from the baseline do the same single plaintext multiply plus extra operations, so they cannot be faster. I'm resubmitting the minimal kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```