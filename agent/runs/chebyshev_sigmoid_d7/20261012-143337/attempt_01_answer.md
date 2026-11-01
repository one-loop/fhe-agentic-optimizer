**Idea:** The baseline evaluates T_k by the linear recurrence, so depth grows to 8. I use the product identities T_{m+n} = 2·T_m·T_n − T_{|m−n|} instead, which keeps the same six ciphertext multiplications but reaches each T_k in fewer levels. T2 and T4 come from squarings, T3 = 2·z·T2 − z, T5 = 2·T2·T3 − z, T6 = 2·T3² − 1 and T7 = 2·T3·T4 − z. The maximum depth of any T_k is 4, so with the final coefficient multiply the circuit uses about 5 levels instead of 8. The coefficient multiplies and the final sum are unchanged.

```cpp
// Chebyshev sigmoid degree 7 on [-4,4], using product identities
// T_{m+n} = 2 T_m T_n - T_{|m-n|} to keep multiplicative depth low.

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  Ciphertext z = x * alpha;            // T1, depth 1

  Ciphertext zz = z * z;               // depth 2
  Ciphertext T2 = zz + zz - one;       // T2

  Ciphertext zT2 = z * T2;             // depth 3
  Ciphertext T3 = zT2 + zT2 - z;       // T3

  Ciphertext T2s = T2 * T2;            // depth 3
  Ciphertext T4 = T2s + T2s - one;     // T4

  Ciphertext T2T3 = T2 * T3;           // depth 4
  Ciphertext T5 = T2T3 + T2T3 - z;     // T5

  Ciphertext T3s = T3 * T3;            // depth 4
  Ciphertext T6 = T3s + T3s - one;     // T6 (unused in the sum, so dropped below)

  Ciphertext T3T4 = T3 * T4;           // depth 4
  Ciphertext T7 = T3T4 + T3T4 - z;     // T7

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");

  Ciphertext term1 = z * c1;
  Ciphertext term3 = T3 * c3;
  Ciphertext term5 = T5 * c5;
  Ciphertext term7 = T7 * c7;

  Ciphertext result = term1 + term3;
  result = result + term5;
  result = result + term7;
  result = result + c0;
  result.set_output("y");
}
```