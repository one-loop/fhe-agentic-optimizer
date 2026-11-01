**Idea:** The cost is dominated by ciphertext-ciphertext multiplies (6 × ~30 ms ≈ 180 ms of the 181 ms total), so the lever is the number of ct-ct multiplies, not the depth. I use a baby-step/giant-step split for the degree-7 polynomial in z. Baby steps are z, z², z³ (z² and z³ are two ct-ct muls). The giant step is z⁴ = z²·z², one more ct-ct mul. The polynomial is p = A + z⁴·B, where A = c0 + c1·z + c2·z² + c3·z³ and B = c4 + c5·z + c6·z² + c7·z³. The final product z⁴·B is one more ct-ct mul, so the total is 4 ct-ct multiplies instead of 6. The depth is about 5 levels. Within each sum, terms sit at distinct levels (z at 2, z² at 3, z³ at 4), so the scale-mismatch error from earlier attempts should not recur.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;

  Ciphertext z2 = z * z;
  Ciphertext z3 = z * z2;
  Ciphertext z4 = z2 * z2;

  // Block A = c1 z + c2 z^2 + c3 z^3 (constant c0 added at the end)
  Plaintext c1("c1");
  Ciphertext a1 = z * c1;
  Plaintext c2("c2");
  Ciphertext a2 = z2 * c2;
  Plaintext c3("c3");
  Ciphertext a3 = z3 * c3;
  Ciphertext A = a1 + a2;
  A = A + a3;

  // Block B = c4 + c5 z + c6 z^2 + c7 z^3
  Plaintext c5("c5");
  Ciphertext b1 = z * c5;
  Plaintext c6("c6");
  Ciphertext b2 = z2 * c6;
  Plaintext c7("c7");
  Ciphertext b3 = z3 * c7;
  Ciphertext B = b1 + b2;
  B = B + b3;
  Plaintext c4("c4");
  B = B + c4;

  // p = A + z^4 * B
  Ciphertext P = z4 * B;
  Ciphertext result = A + P;

  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```