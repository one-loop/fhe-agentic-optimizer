I have no further reduction that stays within the allowed operations, so I'm returning the attempt 3 kernel unchanged.

Attempt 3 is already at the minimum I can reach. The polynomial needs a squaring chain (z², T2², and one more ct-ct product), and the coefficient combinations I would need to remove more multiplies (such as c2−3c6) require plaintext-plaintext arithmetic, which CKKS here doesn't allow. The same restriction blocks avoiding the `alpha` multiply, since α² isn't available as an input.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * c6;
  Ciphertext q = m + m + c4;
  Ciphertext p = t4 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - m;
  result = result + p;
  result = result + c0;
  result.set_output("y");
}
```