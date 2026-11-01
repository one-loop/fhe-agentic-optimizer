I still have no cheaper circuit. The baseline's single plaintext add (2.70 ms) is the minimum for `x0 + p`, and the only different circuit I tried was slower. I'm submitting the baseline kernel again as my final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```