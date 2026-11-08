I have no cheaper circuit than the baseline, so this is the same kernel again. One ct-pt multiply with its rescale is the minimum for x0 * p, and any restructuring I can write under the CKKS rules adds operations. The judge will report it as identical to the baseline, so it cannot win.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```