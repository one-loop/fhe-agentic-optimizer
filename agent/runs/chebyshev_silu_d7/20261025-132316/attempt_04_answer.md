Idea: use Horner form in v = T2. Since c4·T4 = v·(2c4·v) − c4 and c6·T6 = v·c6·(2T4 − 1), the whole bracket factors as v · (c2 + c4·(v+v) + c6·w) − c4, with w = 2T4 − 1. This needs 4 plaintext multiplications instead of 5 and keeps 3 ct-ct multiplications. The pieces being added sit at different levels, so there should be no scale mismatch.

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
  Ciphertext v = u + u - one;
  Ciphertext s = square(v);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 + t4 - one;
  Ciphertext inner = (v + v) * c4 + c2;
  inner = inner + w * c6;
  Ciphertext r = v * inner;
  r = r + z * c1;
  r = r - c4;
  r = r + c0;
  r.set_output("y");
}
```