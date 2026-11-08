I have no cheaper circuit to offer. The task is a single ciphertext + plaintext add, and every other form either normalizes to the same circuit or adds operations. Without a new idea, I'll resubmit the same kernel again. I expect it to be rejected as identical, as in attempts 1 and 2.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```