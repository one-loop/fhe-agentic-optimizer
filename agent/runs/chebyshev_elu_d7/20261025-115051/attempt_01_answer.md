Idea: avoid the sequential recurrence and the many plaintext multiplications. Since all coefficients are plaintext inputs, I fold the constants into a power basis in z only where that is cheap, but the coefficient conversion would need plaintext-plaintext arithmetic, which is not allowed. So I keep the Chebyshev coefficients and reduce the work another way.

Plan:
- z = x*alpha (1 ct-pt mul).
- Compute z2 = z*z, t2 = 2 z2 - 1.
- t4 = 2 t2^2 - 1.
- t3 = 2 z t2 - z.
- t5 = 2 t2 t3 - z.
- t6 = 2 t3^2 - 1.
- t7 = 2 t3 t4 - z.

This gives depth: z at level 1, z2 and t2 at level 2, t3 and t4 at level 3, t5, t6 and t7 at level 4, then the coefficient multiplication makes it 5. That is lower than 8, and the ct-ct multiplication count is z2, t2², z·t2, t2·t3, t3², t3·t4 = 6, the same as the baseline but with less depth.

To cut plaintext multiplications, I group the terms by level so the sums align without extra rescaling. Each term ci·Ti still needs one multiplication, so there are 7 in total. The "-1" and "-z" subtractions are handled with `one` (a plaintext add) and ciphertext subtraction.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext m5 = t2 * t3;
  Ciphertext t5 = m5 + m5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext m7 = t3 * t4;
  Ciphertext t7 = m7 + m7 - z;
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