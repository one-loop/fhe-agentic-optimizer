I have no new circuit. Every correct way to write y = x*A + B reduces to one ct-pt multiply with rescale and one plaintext add. The only ways to change that would need constants or plaintext-plaintext arithmetic, which the CKKS rules forbid. Further resubmissions would be identical, so I'm keeping the baseline kernel as the best answer.

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