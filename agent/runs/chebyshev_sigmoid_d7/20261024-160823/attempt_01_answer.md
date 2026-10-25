Idea: write the odd polynomial in z directly: y = c0 + z(a1 + a3 z² + a5 z⁴ + a7 z⁶), with the coefficients a_k of the monomial basis. However, the plaintext inputs are only the Chebyshev c_k, and plaintext-plaintext arithmetic is not allowed. So the conversion has to happen with ciphertext operands.

T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z, T7 = 64z⁷ − 112z⁵ + 56z³ − 7z.

Doing this with scalars means every multiple needs additions only (no constants). That is costly, so I use a different approach: compute the powers u = z², z³, etc. and apply the Chebyshev coefficients in a structure that reduces ct-ct multiplications.

Using the products T3, T5, T7 via the identities T_{m+n} = 2 T_m T_n − T_{|m−n|}:
- T2 = 2z² − 1 (1 square)
- T3 = 2 z T2 − z (1 mul)
- T4 = 2 T2² − 1 (1 square)
- T7 = 2 T3 T4 − T1 (1 mul)
- T5 = 2 T2 T3 − T1 (1 mul)

That is 5 ct-ct multiplications versus 6, and the depth is lower: z is depth 1 (the plaintext mul), T2 is depth 2, T3 is depth 3, T4 is depth 3, T5 is depth 4, and T7 is depth 4. Then the c-multiplications add one more level. The baseline reaches depth 8, so this is much shallower, which makes the operations cheaper.

To reduce the plaintext multiplications, I fold alpha in: z = x*alpha costs one pt mul, which is needed. The final sum is c1·z + c3·T3 + c5·T5 + c7·T7 + c0, which is 4 pt muls.

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
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a5 = t2 * t3;
  Ciphertext t5 = a5 + a5 - z;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```