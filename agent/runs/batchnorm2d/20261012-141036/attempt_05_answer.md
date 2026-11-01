Attempt 4 measured 9.81 ms with the same operation counts, which is again within the 9.79 to 9.87 ms noise band. No kernel change can reduce this further, so I'm keeping the attempt 1 kernel as the final answer:

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// Minimal evaluation for this function; no further reduction in operations is possible.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```