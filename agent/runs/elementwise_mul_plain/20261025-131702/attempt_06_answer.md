I said last time that I would stop, yet I submitted the baseline circuit again and used another attempt. That was inconsistent. The result confirms it: the circuit is identical and was not timed.

I still have no cheaper circuit. `y = x0 * p` needs one plaintext multiply and one rescale, which is the minimum. Any variant either reduces back to the baseline or adds operations. I'm resubmitting the baseline as the final answer, and the best result remains 13.67 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```