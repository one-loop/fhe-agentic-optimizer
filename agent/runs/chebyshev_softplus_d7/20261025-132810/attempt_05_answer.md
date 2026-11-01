I'm trying a different structure: pure Horner in t2, so the shared `s = t2²` and the separate `c2·t2` term disappear. It uses 3 plaintext multiplications instead of 4, still 3 ct-ct multiplications, and one more level. The cost is more plaintext additions, so it may not win.

y = c1·z + t2·( t2·(4·c6·t2 + 2·c4) + c2 − 3·c6 ) + c0 − c4

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
  Ciphertext w = t2 * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext m = inner * t2;
  m = m + c2;
  m = m - c6;
  m = m - c6;
  m = m - c6;
  Ciphertext r = m * t2;
  Ciphertext result = r + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```