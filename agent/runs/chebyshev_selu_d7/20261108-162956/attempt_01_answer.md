The baseline builds T_k one step at a time, which gives multiplicative depth 8. I use the product identities T_{2k} = 2T_k² − 1 and T_{m+n} = 2·T_m·T_n − T_{|m−n|} instead. That gives T2 = 2z² − 1, T3 = 2z·T2 − z, T4 = 2T2² − 1, T5 = 2T2·T3 − z, T6 = 2T3² − 1 and T7 = 2T3·T4 − z. Every T_k then sits at depth 4 or less, and the whole kernel needs about 5 levels instead of 8. Every operation therefore runs on fewer primes, and fewer level and scale alignments are needed. I have not compiled or timed this kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext t3sq = square(t3);
  Ciphertext t6 = t3sq + t3sq - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```