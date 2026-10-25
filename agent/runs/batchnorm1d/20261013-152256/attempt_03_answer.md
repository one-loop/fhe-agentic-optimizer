This attempt used the same operations as the baseline (one ciphertext-plaintext multiply, one plaintext add), and the 19.02 ms time is within run-to-run noise of 17.74 ms. Splitting the expression into two statements changed nothing in the graph. I'm returning the baseline form, which is the best measured result.

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