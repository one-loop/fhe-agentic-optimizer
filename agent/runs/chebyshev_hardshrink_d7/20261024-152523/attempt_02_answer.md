Idea: switch from Horner to an Estrin split of the same polynomial, P(w) = (q1 + q3·w) + w²·(q5 + q7·w). It uses one more ciphertext multiplication than the previous kernel (a second square for w²). In exchange the multiplicative depth drops from 5 to 4, so the later operations run at cheaper levels. I'm testing whether that trade is a net win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext w2 = square(w);
  Ciphertext Z1 = z * c1;
  Ciphertext Z3 = z * c3;
  Ciphertext Z5 = z * c5;
  Ciphertext Z7 = z * c7;
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext Z7_2 = dbl(Z7);
  Ciphertext Z7_4 = dbl(Z7_2);
  Ciphertext Z7_8 = dbl(Z7_4);
  Ciphertext Z7_16 = dbl(Z7_8);
  Ciphertext Z7_32 = dbl(Z7_16);
  Ciphertext Z7_64 = dbl(Z7_32);
  Ciphertext Z5_2 = dbl(Z5);
  Ciphertext Z5_4 = dbl(Z5_2);
  Ciphertext Z5_16 = dbl(dbl(Z5_4));
  Ciphertext Z3_4 = dbl(dbl(Z3));
  Ciphertext Z3_3 = Z3 + dbl(Z3);
  Ciphertext Z5_5 = Z5 + Z5_4;
  Ciphertext Z5_20 = Z5_16 + Z5_4;
  Ciphertext Z7_7 = Z7_8 - Z7;
  Ciphertext Z7_56 = Z7_64 - Z7_8;
  Ciphertext Z7_112 = dbl(Z7_56);
  Ciphertext q1 = (Z1 - Z3_3) + (Z5_5 - Z7_7);
  Ciphertext q3 = (Z3_4 - Z5_20) + Z7_56;
  Ciphertext q5 = Z5_16 - Z7_112;
  Ciphertext q7 = Z7_64;
  Ciphertext lo = q1 + w * q3;
  Ciphertext hi = q5 + w * q7;
  Ciphertext r = lo + w2 * hi;
  r.set_output("y");
}
```