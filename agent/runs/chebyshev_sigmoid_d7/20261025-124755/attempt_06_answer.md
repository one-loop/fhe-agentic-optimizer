This variant distributes the final multiplication by z to get 5 levels, with one more ciphertext product than attempt 3. Here T3 = z·e3 is computed early at a shallow level. y = T3·w + z·(c5·e5 − c7 + c1) + c0. The two products land at level 5 with the same multiplication pattern, so the scales should match.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext e3 = t + t - one;
  Ciphertext t4 = square(t);
  t4 = t4 + t4 - one;
  Ciphertext e5 = t4 + t4 - e3;
  Ciphertext q = t4 * c7;
  Ciphertext w = q + q + c3;
  Ciphertext T3 = z * e3;
  Ciphertext r = e5 * c5;
  r = r - c7;
  r = r + c1;
  Ciphertext result = T3 * w + z * r;
  result = result + c0;
  result.set_output("y");
}
```