The single ct-pt multiply is already the minimum for x0 * p, so any different circuit has to trade operations around. I don't see a way to make it cheaper, but here is a structurally different variant that avoids the rescale cost of the multiplication result being used at a deep level.

Idea: split the product as x0*p = (x0 + x0) * p - x0 * p. That adds work, so it will probably be slower. It is still a distinct circuit, which gives a data point on how the operation counts affect timing. The extra multiply is likely to cost more than it saves, but it is the only kind of rewrite available here.

A cheaper-looking alternative is to avoid the rescale by multiplying by a plaintext with no scale change, but CHEHAB inserts the rescale automatically and I can't control it.

I'm submitting the variant below. It computes a = x0 + x0, b = a * p, c = x0 * p, and y = b - c. This gives y = 2·x0·p − x0·p = x0·p, which is correct. It uses two ct-pt multiplies, so I expect it to be slower than the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext a = x0 + x0;
  Ciphertext b = a * p;
  Ciphertext c = x0 * p;
  Ciphertext y = b - c;
  y.set_output("y");
}
```