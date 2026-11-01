Nothing has changed since my last message. The baseline's single plaintext add is the minimum circuit for `x0 + p`, and I have no cheaper structure to try. Resubmitting it cannot be judged faster, so there is no point spending more attempts. I'm keeping the baseline as the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```