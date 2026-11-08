I haven't found a cheaper circuit. `y = x*A + B` needs one plaintext multiply, one rescale and one plaintext add. Folding B into the multiply would need a ciphertext of ones or plaintext-plaintext arithmetic, and CKKS rule 2 forbids both. This attempt only swaps the operand order (`b + x*a`), so it will probably be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = b + (a * x);
  y.set_output("y");
}
```