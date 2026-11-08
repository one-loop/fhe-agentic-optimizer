The baseline is already minimal in arithmetic: one plaintext multiply, one rescale and one plaintext add. The only remaining lever is the add of B.

Adding the plaintext B costs about 2.5 ms for encoding. The alternative is to encrypt-free fold B in some other way, but that needs a ciphertext operand, and the only ciphertext is x.

One option is to compute (x + B/A) * A. That needs a plaintext divide, which is not allowed, and it would still need an add of a plaintext vector, so it saves nothing.

Another option is to change the order: the add happens after the rescale, at the deeper level, where it is cheaper. This is probably what already happens. If CHEHAB instead adds B before the rescale, the plaintext would have to be encoded at the doubled scale. Either way the cost is the same.

I can't see a circuit with fewer operations, so I'll try a structurally different form, `(x*a) + b` with explicit ordering. CHEHAB will probably normalize it away, but it is the only variation available.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
```