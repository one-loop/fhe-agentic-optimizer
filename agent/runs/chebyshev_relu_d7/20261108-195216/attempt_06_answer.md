The algebraic structure is already tight. This attempt uses `square()` for the two self-products z·z and w·w instead of generic ciphertext multiplication. SEAL's squaring computes 3 polynomial products instead of 4, so the two squaring steps should be cheaper. Everything else follows attempt 2, which has the fewest plaintext adds.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext w = zz + zz - one;          // T2
  Ciphertext ww = square(w);             // T2^2
  Ciphertext u = w * c6;                 // c6*T2
  Ciphertext inner = u + u + c4;         // 2 c6 T2 + c4
  Ciphertext hi = (ww + ww) * inner;     // 2 T2^2 * inner
  Ciphertext lin = z * c1;
  Ciphertext rest = w * c2 - u - inner + lin + c0;
  Ciphertext result = hi + rest;
  result.set_output("y");
}
```