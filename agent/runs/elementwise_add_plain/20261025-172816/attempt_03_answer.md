The negation variant was slower (5.29 ms against 4.64 ms) because it added two extra operations. The baseline's single plaintext add is already the minimum for this function. Every cheaper circuit would need fewer operations than one add, and that would drop the plaintext term, so I am resubmitting the minimal kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```