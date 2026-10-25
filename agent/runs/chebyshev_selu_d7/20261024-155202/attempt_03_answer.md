This attempt drops the separate `r` term. The r-terms are folded into one product q·(c6·s + 2·z·c7·s), which should shorten the dependency chain for those terms. It costs one extra ciphertext multiplication (5 instead of 4), so it may well come out slower than the current best.

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
  Ciphertext v1 = s + s - one;
  Ciphertext v2 = q + q - v1;
  Ciphertext a6 = s * c6;
  Ciphertext a7 = s * c7;
  Ciphertext m = z * a7;
  // r = 2 s q - s ; c6 r + 2 z c7 r' handled via a6, a7
  Ciphertext b = a6 + m + m;
  Ciphertext rr = q * b;
  Ciphertext rterm = rr + rr - b;
  // odd part without r: c1 + c3 v1 + c5 v2 + c7*(-v2)
  Ciphertext o = v1 * c3 + v2 * c5 + c1;
  Ciphertext v2c7 = v2 * c7;
  Ciphertext e = s * c2 + q * c4 + c0;
  Ciphertext y = e + z * o + rterm - z * v2c7;
  y.set_output("y");
}
```