I'm resubmitting the baseline because it is already the optimal circuit. One ct-pt multiply plus the automatic rescale is the minimum for `x0 * p`, and there is nothing else in it to remove. Any different circuit would add operations and run slower than 7.06 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```