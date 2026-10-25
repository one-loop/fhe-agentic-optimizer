This is the best kernel so far at 199.19 ms, against 307.11 ms for the baseline, and I have no further change I trust to beat it. It is the attempt-4 kernel again, with 3 ct-ct multiplications, 3 ct-pt multiplications and depth 5.

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
  Ciphertext t = u + u - one;
  Ciphertext w = t * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4 + c4;
  Ciphertext m = t * inner;
  Ciphertext g = m + c2 - c6 - c6 - c6;
  Ciphertext r = t * g + z * c1;
  Ciphertext result = r + c0 - c4;
  result.set_output("y");
}
```