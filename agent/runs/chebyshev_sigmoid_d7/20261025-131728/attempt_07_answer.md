Next change: `ww` is only used inside `c7·ww`, so I rewrite `q = c5·w + 2·c7·w² − c7` as `q = w·(2·(c7·w) + c5) − c7`. One plaintext multiplication (`c5·w`) goes away, and the `c5` term becomes a cheap plaintext add. The ciphertext multiplication count stays at 4 and the levels stay the same.

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
  Ciphertext s = z * z;
  Ciphertext w = s + s - one;
  Ciphertext v = w + w - one;
  Ciphertext b = w * c7;
  Ciphertext e = b + b + c5;
  Ciphertext q = w * e;
  q = q - c7;
  Ciphertext r = q + q + c3;
  Ciphertext p = r * v;
  p = p + c1;
  p = p - c5;
  p = p - c7;
  Ciphertext result = z * p;
  result = result + c0;
  result.set_output("y");
}
```