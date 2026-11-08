The idea is to cut the plaintext multiplications and the depth by factoring the even part in w = T2.

Since T6 = w(2·T4 − 1), the even part becomes c0 + (c2 − c6)·w + (c4 + 2·c6·w)·T4. I never form c2 − c6 as plaintext arithmetic. I compute `c2*w` and `q = c6*w` separately and subtract q.

This drops the separate c4·T4 and c6·T6 plaintext multiplications, so there are 4 plaintext multiplications instead of 5. The ciphertext multiplication count stays at 3 (two squares and one product), but the multiplicative depth falls from 5 to 4.

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
  Ciphertext u = square(z);
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext t4 = s + s - one;
  Ciphertext q = w * c6;
  Ciphertext p = q + q + c4;
  Ciphertext r = p * t4;
  r = r + w * c2;
  r = r - q;
  r = r + z * c1;
  r = r + c0;
  r.set_output("y");
}
```