This is the last attempt, and nothing I can see removes another multiplication, plaintext add or alignment. The ideas I checked either cost a ct-ct multiplication or extra plaintext adds, or need plaintext-plaintext arithmetic, which isn't allowed. So this is only a marginal variant that is still a distinct circuit, and it will most likely tie with attempt 3.

It keeps attempt 3's algebra and grouping: one ct-pt multiplication by alpha, two squarings, one product, and the low part c2·T2 − 3c6·T2 − c4 + c1·z combined at level 3 and aligned once. The only change is that `square()` is used for the first squaring only, z². T2² stays a plain `t2 * t2` multiplication.

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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext zz = square(z);           // level 2
  Ciphertext t2 = zz + zz - one;       // level 2
  Ciphertext t2s = t2 * t2;            // level 3
  Ciphertext w4 = t2s + t2s;           // level 3, = T4 + 1

  Ciphertext u = t2 * c6;              // level 3
  Ciphertext u2 = u + u;
  Ciphertext inner = u2 + c4;          // level 3
  Ciphertext prod = w4 * inner;        // level 4

  Ciphertext term2 = t2 * c2;          // level 3
  Ciphertext term1 = z * c1;           // level 2
  Ciphertext low = (term2 - u - inner) + term1; // level 3

  Ciphertext result = prod + low + c0;
  result.set_output("y");
}
```