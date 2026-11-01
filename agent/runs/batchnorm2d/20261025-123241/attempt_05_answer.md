I have no kernel that computes y = x*A + B with a cheaper circuit. After four attempts every form I try reduces to the same circuit: one ct-pt multiply with rescale, and one plaintext add. Anything that removes one of these needs B/A or a constant, which CKKS rules 1 and 2 forbid. Any other structure only adds operations. The baseline kernel is the best I can offer.

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