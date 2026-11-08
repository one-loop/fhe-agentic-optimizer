I have no further change I expect to beat attempt 4, so here is the same kernel again. Every remaining term needs a ciphertext multiply or a plaintext multiply that the polynomial structure already requires.

The circuit is already at 3 ct-ct multiplies, 4 plaintext multiplies and depth 4. The only constant adds left are `one`, `c4` and `c0`. Merging them would need plaintext-plaintext arithmetic, which is not allowed.

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
  Ciphertext b = t2 * c6;
  Ciphertext g = b + b + c4;
  Ciphertext g2 = g + g;
  Ciphertext r = s * g2;
  Ciphertext low = t2 * c2 - b - g;
  r = r + low;
  r = r + w * c1;
  r = r + c0;
  r.set_output("y");
}
```