Attempt 5 measured 9.83 ms with the same operation counts, so it is noise again. The best measured kernel is still attempt 1 (9.79 ms), and I have no change that would reduce its work. I'm keeping it as the final answer:

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