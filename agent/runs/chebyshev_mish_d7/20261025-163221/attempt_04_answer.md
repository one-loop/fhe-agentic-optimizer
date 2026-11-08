The odd part can also go through Clenshaw in w = T2, which drops the T3, T5 and T7 ciphertexts. With the d_k recurrence, the odd part reduces to z·(b0 − b1). Together with the even-part Clenshaw this gives 6 ct-ct multiplications and 3 ct-pt multiplications, down from 7 and 6. The even part ends at level 5 and the odd part at level 6, so they meet at different levels.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;           // T2

  // odd part: z * (b0 - b1), Clenshaw in w
  Ciphertext t7 = w * c7;
  Ciphertext b2 = t7 + t7 + c5;
  Ciphertext p = w * b2;
  Ciphertext b1 = p + p + c3 - c7;
  Ciphertext q = w * b1;
  Ciphertext inner = q + q + c1 - b2 - b1;
  Ciphertext odd = z * inner;

  // even part: Clenshaw in w
  Ciphertext t6 = w * c6;
  Ciphertext e2 = t6 + t6 + c4;
  Ciphertext pe = w * e2;
  Ciphertext e1 = pe + pe + c2 - c6;
  Ciphertext even = w * e1 - e2 + c0;

  Ciphertext R = odd + even;
  R.set_output("y");
}
```