**Idea:** The baseline builds T_k one step at a time, so its multiplicative depth is 8. I use the product identities T_{m+n} = 2·T_m·T_n − T_{|m−n|} to get T2..T7 from fewer sequential steps: T2 = 2z²−1, T3 = 2zT2−z, T4 = 2T2²−1, T5 = 2T2T3−z, T6 = 2T3²−1, T7 = 2T3T4−z. That is still six ciphertext multiplications, but the chain depth drops to about 4 for the T's, plus one level for the coefficient multiply, so the whole kernel uses about 5 levels. Shallower levels make the remaining multiplications cheaper. Doubling is done as `t + t`, so no numeric constants appear.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T3 = 2 z T2 - z
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;

  // T4 = 2 T2^2 - 1
  Ciphertext T2sq = T2 * T2;
  Ciphertext T4 = T2sq + T2sq - one;

  // T6 = 2 T3^2 - 1
  Ciphertext T3sq = T3 * T3;
  Ciphertext T6 = T3sq + T3sq - one;

  // T5 = 2 T2 T3 - z
  Ciphertext T2T3 = T2 * T3;
  Ciphertext T5 = T2T3 + T2T3 - z;

  // T7 = 2 T3 T4 - z
  Ciphertext T3T4 = T3 * T4;
  Ciphertext T7 = T3T4 + T3T4 - z;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;
  Plaintext c2("c2");
  Ciphertext term2 = T2 * c2;
  Plaintext c3("c3");
  Ciphertext term3 = T3 * c3;
  Plaintext c4("c4");
  Ciphertext term4 = T4 * c4;
  Plaintext c5("c5");
  Ciphertext term5 = T5 * c5;
  Plaintext c6("c6");
  Ciphertext term6 = T6 * c6;
  Plaintext c7("c7");
  Ciphertext term7 = T7 * c7;

  Ciphertext result = term1 + term2;
  result = result + term3;
  result = result + term4;
  result = result + term5;
  result = result + term6;
  result = result + term7;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```