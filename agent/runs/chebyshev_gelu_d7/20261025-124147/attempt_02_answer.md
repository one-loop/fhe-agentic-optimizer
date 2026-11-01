Idea: only T1, T2, T4 and T6 matter, so the polynomial is y = c0 + c1·z + P(w), where w = T2 = 2z² − 1. In terms of w, P = w·(c2 + 2·c4·w + c6·(4w² − 3)) − c4. This needs just 3 ciphertext-ciphertext multiplications (z², w², and the final w·inner), against 5 in the baseline.

The previous attempt failed because two terms met at level 4 with different scale histories. This version keeps the terms at different levels, which CHEHAB aligns for free:
- The `c4·w` part sits at level 3.
- The `c6·(4w² − 3)` part sits at level 4.
- The final multiplication by w puts the whole polynomial at level 5, and the `c1·z` term sits at level 2 and aligns for free.

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
  Ciphertext s = w * w;
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext q = s4 - one - one - one;
  Ciphertext b = q * c6;
  Ciphertext a = w * c4;
  Ciphertext a2 = a + a;
  Ciphertext a3 = a2 + c2;
  Ciphertext inner = b + a3;
  Ciphertext p = w * inner - c4;
  Ciphertext r = p + z * c1;
  r = r + c0;
  r.set_output("y");
}
```