The attempt-1 structure (3 ct-ct, 4 scalar ct-pt, depth 4) seems close to minimal. This attempt tests a different trade-off: the coefficient multiplications move to the deepest levels, where they are cheapest.

- **Odd powers first.** It builds z³ = z·z² and z⁵ = z³·z², which is still only 3 ct-ct multiplications. These run at levels 1, 2 and 3 instead of 1, 2 and 3 with heavier operands.
- **Chebyshev terms from additions.** T3 = 4z³ − 3z and T5 = 16z⁵ − 20z³ + 5z are formed using additions only.
- **Coefficients applied last.** c3 multiplies T3 and c5 multiplies T5 at the deepest levels, so those plaintext multiplications operate on small ciphertexts.

This costs one extra level (depth 5), so it is an experiment to see whether the cheaper deep operations outweigh it.

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
  Ciphertext z2 = z * z;
  Ciphertext z3 = z * z2;
  Ciphertext z5 = z3 * z2;

  Ciphertext z_2 = z + z;
  Ciphertext z_3 = z_2 + z;
  Ciphertext z_4 = z_2 + z_2;
  Ciphertext z_5 = z_4 + z;

  Ciphertext z3_2 = z3 + z3;
  Ciphertext z3_4 = z3_2 + z3_2;
  Ciphertext z3_16 = z3_4 + z3_4;
  z3_16 = z3_16 + z3_16;
  Ciphertext z3_20 = z3_16 + z3_4;

  Ciphertext z5_2 = z5 + z5;
  Ciphertext z5_4 = z5_2 + z5_2;
  Ciphertext z5_8 = z5_4 + z5_4;
  Ciphertext z5_16 = z5_8 + z5_8;

  Ciphertext T3 = z3_4 - z_3;
  Ciphertext T5 = (z5_16 - z3_20) + z_5;

  Ciphertext result = (z * c1 + c0) + T3 * c3 + T5 * c5;
  result.set_output("y");
}
```