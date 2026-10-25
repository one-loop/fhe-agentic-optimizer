The current best has 4 ct-ct multiplies, and I don't see a safe way to remove another one. The remaining costs are the three plaintext multiplies on z and the extra plaintext add, and the plaintext coefficients can't be combined without plaintext-plaintext arithmetic. I'm making a small change: the linear terms are subtracted one at a time, which skips the intermediate add. Otherwise the kernel keeps the structure of the last correct attempt.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext u = t2 * c5 + t4 * c7;
  Ciphertext u2 = u + u;
  Ciphertext u3 = u2 + c3;
  Ciphertext r = t3 * u3;
  Ciphertext lin = z * c1 - z * c5 - z * c7;
  Ciphertext result = r + lin;
  result.set_output("y");
}
```