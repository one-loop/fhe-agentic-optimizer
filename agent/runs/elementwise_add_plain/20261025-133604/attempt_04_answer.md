I don't have a cheaper circuit to propose. The baseline's single plaintext add is the minimum for `x0 + p`, and the two variants so far confirm it: the identical circuit can't win, and the negation variant added two operations and was slower. Resubmitting the same kernel would only use up another attempt, so I'm stopping here with the baseline as the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```