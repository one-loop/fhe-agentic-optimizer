The previous result matches the baseline within noise, so the gain was just measurement variance. The graph is already minimal: one plaintext multiply with rescale, plus one plaintext add.

One idea is to add B before the multiply, as (x + B/A) * A. That needs plaintext-plaintext division, which isn't allowed, and it adds an operation anyway. Another is to add B at the deeper level after the rescale. That is what CHEHAB already does, and encoding B at the lower level is slightly cheaper.

I don't see a legitimate way to remove an operation, so I'm resubmitting the same minimal form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```