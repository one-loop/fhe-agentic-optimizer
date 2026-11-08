Idea: switch to the monomial basis, which needs only 5 ct×ct multiplications, and recover the Chebyshev combinations with integer multiples built from ciphertext additions.
- Compute s = z², z³ = s·z, s² = s·s, z⁵ = s²·z and z⁷ = s²·z³.
- Build T3 = 4z³ − 3z, T5 = 16z⁵ − 20z³ + 5z and T7 = 64z⁷ − 112z⁵ + 56z³ − 7z from integer multiples of the monomials, using adds only. This avoids numeric constants.
- Apply the plaintext coefficients c1, c3, c5, c7 and add c0 as before.

```cpp
Ciphertext imul(const Ciphertext &c, int n)
{
  // n >= 1, built from additions only
  Ciphertext acc;
  Ciphertext cur = c;
  bool has = false;
  while (n > 0)
  {
    if (n & 1)
    {
      if (has)
        acc = acc + cur;
      else
      {
        acc = cur;
        has = true;
      }
    }
    n >>= 1;
    if (n > 0)
      cur = cur + cur;
  }
  return acc;
}

void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext z3 = s * z;
  Ciphertext s2 = square(s);
  Ciphertext z5 = s2 * z;
  Ciphertext z7 = s2 * z3;
  Ciphertext t3 = imul(z3, 4) - imul(z, 3);
  Ciphertext t5 = imul(z5, 16) + imul(z, 5) - imul(z3, 20);
  Ciphertext t7 = imul(z7, 64) + imul(z3, 56) - imul(z5, 112) - imul(z, 7);
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result + t5 * c5;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```