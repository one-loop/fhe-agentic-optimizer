I have no further structural change that I expect to be cheaper, so I'm keeping the attempt 2 kernel as my answer.

The remaining terms (`z*c1`, `w*c2`, `w*c6`, `t4*q`, plus the plaintext adds for `one`, `c4` and `c0`) each need their own plaintext multiplication or addition. I could not merge them, because combining plaintext coefficients (such as c2 − c6) is plaintext-plaintext arithmetic, which CKKS here forbids.

The variants I considered, such as expressing the lower terms through `u` instead of `w`, would only move work around or add plaintext operations. Attempt 3 showed that removing the `t4` subtraction changed the circuit but gained only about 2% (not confirmed as faster).

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
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;
  Ciphertext w2 = w * w;
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext c6w = w * c6;
  Ciphertext q = c6w + c6w + c4;
  Ciphertext p = t4 * q;
  Ciphertext term2 = w * c2;
  Ciphertext term1 = z * c1;
  Ciphertext r = term2 - c6w;
  r = r + term1;
  Ciphertext result = p + r;
  result = result + c0;
  result.set_output("y");
}
```