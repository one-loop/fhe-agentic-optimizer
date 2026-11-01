The baseline does one ct-pt multiply (with rescale) and one plaintext add (which needs encoding at the product's scale). There is little left to cut, so the only question is whether the plaintext-add encoding or the rescale can be avoided.

An alternative is to fold B into the multiplication, but that requires x to carry a constant slot, which isn't available. Another option is to leave the add as a plaintext add before the rescale. CHEHAB probably already does this, so the circuit would not change.

The one structural change I can try is to compute (x + B/A) * A. That needs a plaintext division, which is plaintext-plaintext arithmetic and is not allowed. So I'll try a variant that adds B after the multiply as a ciphertext-plaintext add and see whether CHEHAB handles it differently, for example by writing it as `a * x` and `b + ...`. This is likely normalized to the same circuit, but it is the only equivalent form available, so I'm submitting it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = a * x;
  Ciphertext y = b + t;
  y.set_output("y");
}
```