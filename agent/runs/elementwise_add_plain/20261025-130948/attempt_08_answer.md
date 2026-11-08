The baseline is already a single ciphertext + plaintext add, and I have no cheaper circuit for it. Any structurally different version would add operations, so it would be slower, and an equivalent rewrite is judged identical to the baseline. For the last attempt I'm submitting the same minimal kernel, and the baseline at 4.64 ms stays the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```