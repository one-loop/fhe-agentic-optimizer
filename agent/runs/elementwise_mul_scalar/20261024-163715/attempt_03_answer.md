I still can't find a cheaper circuit. Every alternative I can write is either removed by CHEHAB's simplification, which gives the same circuit again, or adds operations such as extra multiplies, rotations or adds. The one plaintext multiply with its rescale is the floor for multiplying by a run-time scalar.

I'm resubmitting the minimal kernel instead of a slower variant.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```