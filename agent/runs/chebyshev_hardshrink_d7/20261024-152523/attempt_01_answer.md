Idea: the result is an odd polynomial in z, so y = z·P(z²) with P of degree 3 in w = z². That needs far fewer ciphertext multiplications than the baseline's recurrence.

Expand the odd Chebyshev terms with a = c1, b = c3, c = c5, d = c7:
- T3 = 4z³ − 3z
- T5 = 16z⁵ − 20z³ + 5z
- T7 = 64z⁷ − 112z⁵ + 56z³ − 7z

The scalar constants such as 4, 16 and 64 can't be written directly, because CKKS allows no numeric constants. They have to be built from the plaintext inputs by repeated additions, and plaintext-plaintext arithmetic is not allowed. So I fold the constants into ciphertext-side additions instead.

Plan:
- Use z = x·alpha, which costs one plaintext multiplication.
- Compute w = z², which is one ciphertext multiplication.
- Group the polynomial as z·(e1 + e3·w + e5·w² + e7·w³). The coefficients e_k are combinations of the c's, but they must be formed from the plaintext inputs, and plaintext-plaintext arithmetic is not allowed.
- Instead apply the c's to ciphertext terms, so every product has a ciphertext operand.

Evaluation uses a ciphertext u = c1 + (…), scaled by plaintexts:
- Build the multiples of w by additions only: w2 = w + w, w4 = w2 + w2, and so on.
- Term3 = c3·(4z³ − 3z).
- Collect by monomial:
  - z³ coefficient: 4c3 − 20c5 + 56c7
  - z⁵ coefficient: 16c5 − 112c7
  - z⁷ coefficient: 64c7
  - z coefficient: c1 − 3c3 + 5c5 − 7c7

Each coefficient is a linear combination of ciphertext terms. Computing c3·(ciphertext) costs one plaintext multiplication each.

Define the helper polynomials:
- A = c3·w (ct-pt), B = c5·w, C = c7·w
- Then 4c3 − 20c5 + 56c7 is formed through ciphertext additions of multiples of c3, c5 and c7.

To keep this simple, I'll evaluate y = z·(q1 + w·(q3 + w·(q5 + w·q7))) with:
- q1 = c1·1 − 3c3 + 5c5 − 7c7
- q3 = 4c3 − 20c5 + 56c7
- q5 = 16c5 − 112c7
- q7 = 64c7

Building q_k means multiplying the plaintexts c_k by `one`, which needs a ciphertext. I take the ciphertext `onec = alpha·x` form, so this costs extra plaintext multiplications. The cheaper route is to compute c3·z, c5·z and c7·z as ciphertexts (3 plaintext multiplications), then form the combinations by additions:
- Z1 = c1·z, Z3 = c3·z, Z5 = c5·z, Z7 = c7·z
- q1z = Z1 − 3Z3 + 5Z5 − 7Z7
- q3z = 4Z3 − 20Z5 + 56Z7
- q5z = 16Z5 − 112Z7
- q7z = 64Z7

Then y = q1z + w·(q3z + w·(q5z + w·q7z)), which needs 3 ciphertext multiplications plus the square. The multiples are built by additions, which are cheap.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext w = square(z);
  Ciphertext Z1 = z * c1;
  Ciphertext Z3 = z * c3;
  Ciphertext Z5 = z * c5;
  Ciphertext Z7 = z * c7;
  auto dbl = [](const Ciphertext &a) { return a + a; };
  auto mul = [&](const Ciphertext &a, int n) {
    Ciphertext r = a;
    for (int i = 1; i < n; ++i)
      r = r + a;
    return r;
  };
  Ciphertext Z7_2 = dbl(Z7);
  Ciphertext Z7_4 = dbl(Z7_2);
  Ciphertext Z7_8 = dbl(Z7_4);
  Ciphertext Z7_16 = dbl(Z7_8);
  Ciphertext Z7_32 = dbl(Z7_16);
  Ciphertext Z7_64 = dbl(Z7_32);
  Ciphertext Z5_2 = dbl(Z5);
  Ciphertext Z5_4 = dbl(Z5_2);
  Ciphertext Z5_16 = dbl(dbl(Z5_4));
  Ciphertext Z3_4 = dbl(dbl(Z3));
  Ciphertext Z3_3 = Z3 + dbl(Z3);
  Ciphertext Z5_5 = Z5 + Z5_4;
  Ciphertext Z5_20 = Z5_16 + Z5_4;
  Ciphertext Z7_7 = Z7_8 - Z7;
  Ciphertext Z7_56 = Z7_64 - Z7_8;
  Ciphertext Z7_112 = dbl(Z7_56);
  Ciphertext q1 = (Z1 - Z3_3) + (Z5_5 - Z7_7);
  Ciphertext q3 = (Z3_4 - Z5_20) + Z7_56;
  Ciphertext q5 = Z5_16 - Z7_112;
  Ciphertext q7 = Z7_64;
  (void)mul;
  Ciphertext r = q5 + w * q7;
  r = q3 + w * r;
  r = q1 + w * r;
  r.set_output("y");
}
```