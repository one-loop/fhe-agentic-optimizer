I have no cheaper circuit to offer. `y = x*A + B` already uses the minimum: one plaintext multiply, one rescale and one plaintext add. Every algebraic rewrite that could remove one of these needs a constant or plaintext-plaintext arithmetic, which are not allowed. The rewrites I tried either normalize to the baseline circuit (attempts 1, 3, 4, 5) or add operations and run slower (attempt 2). I'm submitting the baseline form again.

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