The even part of the polynomial only needs T2, T4 and T6, so I compute them from T2 alone and cut the depth from 7 to 4.

- z = x·alpha, T2 = 2z² − 1, and T4 = 2·T2² − 1.
- T6 = 2·T2·T4 − T2, so c6·T6 = 2·(c6·T2)·T4 − c6·T2. The plaintext multiply by c6 happens on T2 before the ciphertext product.
- That makes 3 ciphertext-ciphertext multiplications instead of 5 (z², T2², and (c6·T2)·T4).
- The odd term c1·z comes straight from z.

I have not compiled or run this, so the scale alignment of the final sum is untested.

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
  Ciphertext p = a * t4;
  Ciphertext result = p + p - a;
  result = result + t4 * c4;
  result = result + t2 * c2;
  result = result + z * c1;
  result = result + c0;
  result.set_output("y");
}
```