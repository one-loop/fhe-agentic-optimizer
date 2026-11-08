The output is an odd polynomial in z: y = c1 z + c3 T3 + c5 T5 + c7 T7. Rewriting it in the monomial basis gives y = z·P(z²), where P has 4 coefficients: a1 + a3 u + a5 u² + a7 u³, with u = z². That would need coefficients computed from c's, which is plaintext-plaintext arithmetic and not allowed. So I use the Chebyshev identities directly with ciphertext operations only.

The identities are:
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

Computing the monomial coefficients needs constants such as 4 or 3, which we can only build by additions of ciphertexts or plaintexts. Multiplying the c's by integer constants is plaintext-plaintext arithmetic, so it is not allowed.

Instead I keep the Chebyshev structure and reduce cost:
- Let u = z² (one multiplication). Then T2 = 2u − 1.
- T3 = z(2T2 − 1)... this is still a chain.

A cheaper route uses the doubling identities T_{m+n} = 2 T_m T_n − T_{|m−n|}:
- T4 = 2 T2² − 1
- T3 = 2 T2 z − z
- T7 = 2 T4 T3 − z
- T5 = 2 T3 T2 − z

This gives depth: z (1, plaintext mult), T2 (2), T3 (3), T4 (3), T5 (4), T7 (4), then the c multiplications (5). The total is lower than the baseline's 8.

Multiplication counts: z (plaintext mult), T2 (square), T3, T4, T5, T7 (four ct-ct), for 5 ct-ct total versus 6 in the baseline. Depth is 5 versus 8, so the rest of the circuit runs at deeper levels and is cheaper.

Since x·alpha costs a level, I keep it. The "one" plaintext appears only in T2 and T4. The −1 in T4 is expressed as subtracting `one`, which is a plaintext-ciphertext subtraction and is allowed.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = t2 * t2;
  Ciphertext t4 = s4 + s4 - one;
  Ciphertext a5 = t3 * t2;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext a7 = t4 * t3;
  Ciphertext t7 = a7 + a7 - z;
  Ciphertext r = z * c1 + t3 * c3 + t5 * c5 + t7 * c7;
  r.set_output("y");
}
```