Idea: write the polynomial in the monomial basis, y = c0 + c1·z + c3·(4z³−3z) + c5·(16z⁵−20z³+5z). With u = z² and w = z·u = z³, the z⁵ term is 16·c5·z⁵ = 16·(c5·u)·w. That needs only 3 ct-ct multiplications (u, w, and (c5·u)·w) instead of 4. All integer scalings are done by additions, and the coefficients are applied by cheap ct-pt multiplies.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = z * u;
  Ciphertext t = u * c5;
  Ciphertext tw = t * w;
  Ciphertext tw2 = tw + tw;
  Ciphertext tw4 = tw2 + tw2;
  Ciphertext tw8 = tw4 + tw4;
  Ciphertext tw16 = tw8 + tw8;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext z2 = z + z;
  Ciphertext z4 = z2 + z2;
  Ciphertext z3x = z4 - z;
  Ciphertext z5 = z4 + z;
  Ciphertext w16 = w4 + w4;
  w16 = w16 + w16;
  Ciphertext w20 = w16 + w4;
  Ciphertext e3 = w4 - z3x;
  Ciphertext e5 = z5 - w20;
  Ciphertext r = z * c1 + e3 * c3 + e5 * c5 + tw16 + c0;
  r.set_output("y");
}
```