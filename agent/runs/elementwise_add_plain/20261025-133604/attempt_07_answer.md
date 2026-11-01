The baseline is still the best I have: a single plaintext add, 2.70 ms, and I can't find a cheaper circuit for `x0 + p`. The one structurally different variant I tried (negations around the add) was slower at 3.10 ms. Since the identical circuit can't win, I'm not spending the remaining attempts on it. The baseline kernel stays as my answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```