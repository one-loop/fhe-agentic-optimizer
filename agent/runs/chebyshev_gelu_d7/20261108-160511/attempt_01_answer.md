The odd part of the polynomial is only c1·z, so the rest is an even polynomial in w = T2(z) = 2z² − 1. Since T4 = 2w² − 1 and T6 = 4w³ − 3w, I evaluate it as follows:

- c6·T6 = (c6·w)·(4w² − 3), where c6·w is computed in parallel with w²
- c4·T4 = c4·(2w² − 1)

This brings the multiplicative depth from 7 to 4 and the ciphertext-ciphertext multiplications from 5 to 3. I have not compiled or run it, so the scale alignment of the final sum is the part most likely to need a fix.

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
  Ciphertext w = u + u - one;          // T2
  Ciphertext s = w * w;                // w^2
  Ciphertext s2 = s + s;
  Ciphertext s4 = s2 + s2;
  Ciphertext q = s4 - one - one - one; // 4w^2 - 3
  Ciphertext c6w = w * c6;
  Ciphertext t6 = c6w * q;             // c6*T6
  Ciphertext t4 = (s2 - one) * c4;     // c4*T4
  Ciphertext t2 = w * c2;
  Ciphertext t1 = z * c1;
  Ciphertext result = t6 + t4;
  result = result + t2;
  result = result + t1;
  result = result + c0;
  result.set_output("y");
}
```