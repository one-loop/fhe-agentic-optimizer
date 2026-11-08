The last attempt is a plain power chain: `w = z²`, `z3 = z·w`, `z5 = w·z3`, `z7 = w·z5`, which is still 4 ct-ct multiplications. The 4 coefficient multiplications then run at deeper, cheaper levels and no integer-weighted sums are needed, so the add count falls to 3. The terms are accumulated in ascending level order to avoid scale mismatches.

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
  Ciphertext z3 = z * w;
  Ciphertext z5 = w * z3;
  Ciphertext z7 = w * z5;
  Ciphertext r = z * c1;
  r = r + z3 * c3;
  r = r + z5 * c5;
  r = r + z7 * c7;
  r.set_output("y");
}
```

This is mathematically the monomial expansion and I haven't checked it numerically. The monomial coefficients are the integer combinations of `c_k` from before, so this form is not equivalent to `y`. It computes `c1·z + c3·z³ + c5·z⁵ + c7·z⁷` and is incorrect for the Chebyshev function.

I couldn't fix this in the final attempt: the correct weights (`−3c3`, `+5c5`, and so on) need integer multiples of `c_k·z_k` terms, and the four plaintext multiplications each sit on a different power. A correct version would build those weighted sums from `c_k·z_j` pairs, which needs more plaintext multiplications. The best kernel remains attempt 3 (268.18 ms vs. 318.32 ms baseline, 4 ct-ct multiplications, 6 levels).