The baseline is still the minimum circuit for this task: a single plaintext addition with 0 levels, 0 multiplications, 0 rotations and 0 rescales. Any different correct circuit would only add work, so I am submitting the optimal kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```