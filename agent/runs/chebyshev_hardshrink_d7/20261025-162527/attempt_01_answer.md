The target is an odd polynomial in z: y = a1·z + a3·z³ + a5·z⁵ + a7·z⁷, where the a's are the power-basis coefficients. Those coefficients would have to be computed from c1, c3, c5, c7, and plaintext-plaintext arithmetic is not allowed. So I keep the Chebyshev form and look for a cheaper evaluation.

With u = z², the odd Chebyshev polynomials are:
- T3 = 4z³ − 3z = z(4u − 3)
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

Numeric constants can't be written, and "one" is the only plaintext constant available. Powers of 2 come from additions, but 3, 5 and 7 need combinations of adds.

A cleaner route uses the identities T3 = 2z·T2 − z, T5 = 2·T2·T3 − T1 and T7 = 2·T3·T4 − T1, which reduce depth. In terms of cost:
- z = x·alpha costs one plaintext multiply (level 1).
- T2 = 2z² − 1 costs one ct-ct multiply (level 2).
- T3 = 2z·T2 − z is level 3.
- T4 = 2T2² − 1 is level 3.
- T5 = 2·T2·T3 − z is level 4.
- T7 = 2·T3·T4 − z is level 4.

Multiplications: z², z·T2, T2², T2·T3, T3·T4, which is 5 ct-ct multiplies instead of 6, with depth 4 plus the coefficient multiply. I can fold alpha in cheaply: z² = (x·x)·alpha² would need alpha², which is plaintext-plaintext. Instead I compute z = x·alpha, then z².

Coefficient multiplies: c1·z, c3·T3, c5·T5, c7·T7 cost 4 plaintext multiplies, and they are the same as the baseline. The levels total about 5 instead of 8, so later operations are cheaper.

Sums: the terms c1·z (level 2), c3·T3 (level 4), and c5·T5 and c7·T7 (level 5) have different histories, and CHEHAB aligns operands at different levels automatically.

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
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result.set_output("y");
}
```