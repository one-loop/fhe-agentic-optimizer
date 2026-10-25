I still can't find a cheaper circuit. The baseline is already a single ciphertext-plaintext add, which is the minimum for `x0 + p`, and every structural change I can think of adds operations and costs more. For this last attempt I'm submitting the baseline form again. The best result for this task remains the baseline at 4.63 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```