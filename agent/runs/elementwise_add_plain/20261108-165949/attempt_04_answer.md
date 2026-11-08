I have no cheaper circuit to propose. The baseline is a single plaintext add, the minimum possible for this task, and any structurally different version needs extra operations. Resubmitting it again would give the same result as the last three attempts, so the baseline stays the best kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```