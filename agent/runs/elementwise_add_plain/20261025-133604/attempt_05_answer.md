I'm stopping here. The baseline, a single plaintext add, is the best circuit I can find. Every alternative adds operations (the negation variant measured 3.10 ms against 2.70 ms) or normalizes back to the baseline circuit, and the identical circuit can't be judged faster. Nothing available to the DSL lowers the encoding cost of `p` without extra operations. I'm giving the baseline kernel as my final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```