Idea: write the odd polynomial in z as p(z) = c0 + z·(a1 + a3 z² + a5 z⁴ + a7 z⁶) and evaluate it in monomial form, which needs only a few multiplications.

The Chebyshev odd terms expand as follows:
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

Numeric constants are not allowed, and the plaintext inputs only give c_k, alpha and one. Combining coefficients such as 4·c3 would need plaintext-plaintext arithmetic, which is also not allowed. So I stay with the Chebyshev structure, but use the product identities to cut depth and multiplications:
- T2 = 2z² − 1
- T3 = z(2T2 − 1) − z... this form still needs constants, so I avoid it.
- Use T_{m+n} = 2 T_m T_n − T_{|m−n|}.

Plan, with u = x·alpha as the first multiplication:
- s = z² (one ct×ct multiplication), T2 = s + s − one.
- T4 = 2 T2² − 1 (one multiplication).
- T3 = 2 z T2 − z.
- T5 = 2 T2 T3 − z.
- T7 = 2 T4 T3 − z.

That is five ct×ct multiplications for the powers, which is still many. A cheaper route is to fold the coefficients into the multiplications by evaluating the polynomial in s = z²:

p = c0 + z·(c1 + c3·T3/z + …)

Constants block that approach. Instead I reduce depth by fusing the alpha scaling into later steps:
- z = x·alpha (1 plaintext multiplication).
- s = z·z (1 ct×ct multiplication).
- T2 = 2s − one.
- A = c1 + c3·(2T2 − 1) + …

That again requires constants such as the 2 in 2T2 − 1, so I stay with the doubling-by-addition form and the product identities.

Final structure:
- z, then T2 = 2z² − 1.
- T3 = 2·z·T2 − z, T4 = 2·T2² − 1.
- T5 = 2·T4·z − T3, T7 = 2·T4·T3 − z.

This is 5 ct×ct multiplications (z·z, z·T2, T2·T2, T4·z, T4·T3) against 6 in the baseline. Depth is lower because T4 and T3 sit at similar levels, and the four coefficient multiplications stay as they are.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a5 = t4 * z;
  Ciphertext t5 = a5 + a5 - t3;
  Ciphertext a7 = t4 * t3;
  Ciphertext t7 = a7 + a7 - z;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```