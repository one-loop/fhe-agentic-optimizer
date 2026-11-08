The Chebyshev basis forces several terms to the same level with different scale histories, so I switch to Clenshaw evaluation.

The recurrence is b_k = c_k + 2z·b_{k+1} − b_{k+2}, with y = c0 + z·b1 − b2. It needs only the ciphertext-ciphertext multiplies in the chain (5 for b5…b1, plus 1 for the final step, so 6 in total). It needs just 2 plaintext multiplies (`x*alpha` and `z*c7`), instead of the baseline's 8. Each b_k sits at its own level, so no same-level scale conflicts should arise.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext zc7 = zz * c7;
  Ciphertext b6 = zc7 + c6;
  Ciphertext b5 = zz * b6 + c5 - c7;
  Ciphertext b4 = zz * b5 + c4 - b6;
  Ciphertext b3 = zz * b4 + c3 - b5;
  Ciphertext b2 = zz * b3 + c2 - b4;
  Ciphertext b1 = zz * b2 + c1 - b3;
  Ciphertext y = z * b1 + c0 - b2;
  y.set_output("y");
}
```