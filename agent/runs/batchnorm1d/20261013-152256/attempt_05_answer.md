This attempt also ran the same operations (one ciphertext-plaintext multiply, one plaintext add) and measured 18.05 ms, again within the 17.74–19.02 ms noise band. There's no change in the computation that could reduce the cost, so I'm keeping the baseline as the final kernel.

```cpp
// y = x * A + B: one ciphertext-plaintext multiply and one plaintext add.
// This is the minimal evaluation; no rotations or ciphertext multiplications.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```