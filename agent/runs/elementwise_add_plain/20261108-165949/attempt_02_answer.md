I don't see a cheaper circuit. The output needs one operation that combines `x0` with the plaintext `p`, and a single plaintext add is the cheapest such operation. Every alternative I can think of (negating twice, splitting the add, or going through a multiply) adds operations or consumes a level. I'm submitting the same minimal kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```