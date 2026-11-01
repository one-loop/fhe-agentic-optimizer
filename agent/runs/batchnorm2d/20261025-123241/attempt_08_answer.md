I have no cheaper circuit for this last attempt. The baseline's single ct-pt multiply with rescale and one plaintext add is the minimum for y = x*A + B with arbitrary per-slot A and B. Removing either operation would need constants or plaintext-plaintext arithmetic, which the rules forbid. The baseline remains the best kernel I can offer.

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