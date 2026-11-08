Idea: compute b = c6·T2 early, then fold the 2·T2·T4 − T2 structure of c6·T6 into a single product m = b·(2·T4 − 1). This drops one plaintext multiply and a few adds, and cuts the multiplicative depth from 5 to 4.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext b = t2 * c6;
  Ciphertext v = t4 + t4 - one;
  Ciphertext r = b * v;
  r = r + t4 * c4;
  r = r + t2 * c2;
  r = r + w * c1;
  r = r + c0;
  r.set_output("y");
}
```