Next change: rewrite the polynomial as `y = c0 + z·P(w)`. Here `v = 2w − 1` and `P = (c3 + 2q)·v + c1 − c5 − c7`. The constant `c1 − c5 − c7` is applied as plaintext subtractions on a ciphertext, which are legal and cheap. This removes `zw`, `T3` and the three `z·c` multiplications. It uses 4 ct-ct multiplications and 3 pt multiplications, against 6 in the previous best. Depth rises to 6, which is still within the limit.

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
  Ciphertext ww = w * w;
  Ciphertext t4 = ww + ww - one;
  Ciphertext q = w * c5 + t4 * c7;
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