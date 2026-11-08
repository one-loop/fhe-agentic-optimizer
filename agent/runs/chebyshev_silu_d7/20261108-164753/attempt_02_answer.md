The scale mismatch came from adding `c4*T4` and `(c6*T2)*T4` at level 4 with different multiplication histories. I fold the c4 term into the product: `T4*(2a + c4)`, where `a = c6*T2` and the plaintext add of c4 sits at level 3. This removes the separate `c4*T4` multiply. The remaining terms (`c2*T2 - a`, `c1*z`) enter at lower levels, so CHEHAB can align them for free. I have not recompiled this version.

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
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext p = m * t4;
  Ciphertext d = t2 * c2 - a;
  Ciphertext w = z * c1 + c0;
  Ciphertext result = p + (d + w);
  result.set_output("y");
}
```