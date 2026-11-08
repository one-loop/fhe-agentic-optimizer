Attempt 6 produced the same circuit as the baseline again. The baseline is already the cheapest possible circuit for this task: one multiply by A, one rescale and one add of B. Any different circuit would add work and be slower, so the baseline stays as the final answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```