I have no further circuit to propose. The baseline's single ct-pt multiply with rescale and one plaintext add is already the minimum for y = x*A + B with general per-slot A and B, and every rewrite I can construct either normalizes back to it or adds work. I'm keeping the baseline kernel as my answer.

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