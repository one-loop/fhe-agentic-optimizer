Idea: keep the attempt-1 Horner structure, which is still the fastest, but build the integer multiples of Z3, Z5 and Z7 with a leaner chain of additions. Each q is formed in place from a shared Z7_7 = 7·Z7, which means fewer ciphertext adds. The multiplication structure is unchanged, so any gain will be small.

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
  Ciphertext Z1 = z * c1;
  Ciphertext Z3 = z * c3;
  Ciphertext Z5 = z * c5;
  Ciphertext Z7 = z * c7;
  auto dbl = [](const Ciphertext &a) { return a + a; };
  Ciphertext Z7_8 = dbl(dbl(dbl(Z7)));
  Ciphertext Z7_7 = Z7_8 - Z7;
  Ciphertext Z7_14 = dbl(Z7_7);
  Ciphertext Z7_28 = dbl(Z7_14);
  Ciphertext Z7_56 = dbl(Z7_28);
  Ciphertext Z7_112 = dbl(Z7_56);
  Ciphertext Z7_64 = Z7_56 + Z7_8;
  Ciphertext Z5_4 = dbl(dbl(Z5));
  Ciphertext Z5_5 = Z5 + Z5_4;
  Ciphertext Z5_20 = dbl(dbl(Z5_5));
  Ciphertext Z5_16 = Z5_20 - Z5_4;
  Ciphertext Z3_4 = dbl(dbl(Z3));
  Ciphertext Z3_3 = Z3_4 - Z3;
  Ciphertext q1 = (Z1 - Z3_3) + (Z5_5 - Z7_7);
  Ciphertext q3 = (Z3_4 - Z5_20) + Z7_56;
  Ciphertext q5 = Z5_16 - Z7_112;
  Ciphertext r = q5 + w * Z7_64;
  r = q3 + w * r;
  r = q1 + w * r;
  r.set_output("y");
}
```