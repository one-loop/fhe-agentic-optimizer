Idea: T4 = 2v² − 1 and T6 = 4v³ − 3v with v = T2, so c4·T4 + c6·T6 = v²·(2c4 + 4c6·v) − c4 − 3c6·v. This multiplies s2 = v² directly by w2 = 2·(2·c6·v + c4), which avoids building T4 and its adds. The ct-ct and ct-pt multiplication counts stay the same.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s2 = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext w = a + a + c4;
  Ciphertext w2 = w + w;
  Ciphertext prod = s2 * w2;
  Ciphertext low = t2 * c2 - a - a - a;
  low = low + z * c1;
  low = low - c4;
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
```