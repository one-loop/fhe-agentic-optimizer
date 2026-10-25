I have no further change that I expect to beat attempt 2, which is 302 ms against the 358 ms baseline. The merged-`r` variants either added a ciphertext multiplication or hit scale mismatches. So I'm resubmitting the attempt-2 kernel unchanged.

It writes the even part as a polynomial in s = T2(z) and the odd part as z times a polynomial in s. Both use only 4 ciphertext multiplications and 7 plaintext multiplications.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext s = z2 + z2 - one;
  Ciphertext s2 = s * s;
  Ciphertext q = s2 + s2 - one;
  Ciphertext sq = s * q;
  Ciphertext r = sq + sq - s;
  Ciphertext v1 = s + s - one;
  Ciphertext v2 = q + q - v1;
  Ciphertext v3 = r + r - v2;
  Ciphertext e = s * c2 + q * c4 + r * c6 + c0;
  Ciphertext o = v1 * c3 + v2 * c5 + v3 * c7 + c1;
  Ciphertext y = e + z * o;
  y.set_output("y");
}
```