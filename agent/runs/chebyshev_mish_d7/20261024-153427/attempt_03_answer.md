This attempt uses Clenshaw's recurrence instead of building each T_k. It evaluates the polynomial with the same six ciphertext multiplications as the baseline. Only 2 plaintext multiplications remain (alpha·x and z·c7) instead of 8. Each b_k combines terms from different levels, so CHEHAB can align them for free.

The recurrence is b_k = c_k + 2z·b_{k+1} − b_{k+2}, with b7 = c7 and the result y = c0 + z·b1 − b2.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;

  Ciphertext p6 = z * c7;
  Ciphertext b6 = p6 + p6 + c6;

  Ciphertext m5 = z * b6;
  Ciphertext b5 = m5 + m5 + c5 - c7;

  Ciphertext m4 = z * b5;
  Ciphertext b4 = m4 + m4 + c4 - b6;

  Ciphertext m3 = z * b4;
  Ciphertext b3 = m3 + m3 + c3 - b5;

  Ciphertext m2 = z * b3;
  Ciphertext b2 = m2 + m2 + c2 - b4;

  Ciphertext m1 = z * b2;
  Ciphertext b1 = m1 + m1 + c1 - b3;

  Ciphertext m0 = z * b1;
  Ciphertext y = m0 + c0 - b2;
  y.set_output("y");
}
```