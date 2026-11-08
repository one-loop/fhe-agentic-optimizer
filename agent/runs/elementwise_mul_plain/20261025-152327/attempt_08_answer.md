The baseline is still the best I can do. The task is one ct-pt multiply with its rescale, and I can't find a cheaper equivalent circuit. Constants and plaintext-plaintext arithmetic are forbidden, and every other form adds a multiply, a rotation or an alignment. I'm submitting it unchanged for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```