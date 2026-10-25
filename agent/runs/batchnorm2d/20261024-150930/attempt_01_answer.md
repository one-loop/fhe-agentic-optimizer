The baseline does one plaintext multiply with a rescale, then one plaintext add (which costs encoding time). The computation is y = x*A + B, and with only one ciphertext input and plaintext coefficients, the multiply by A cannot be avoided.

The one saving I can see is the add of B. A plaintext add needs an encoding at the scale and level of the product, which costs about 2.5 ms. The alternatives I can think of don't beat that:
- Encoding B directly as a ciphertext isn't possible, because B is a plaintext input.
- Folding B into the multiply would need a constant-one ciphertext, which would cost more.

I'll try the version with the add placed before the rescale, which CHEHAB may already do. Writing it as `x * a + b` already gives that structure. A different circuit that might help is computing `(x + b/a) * a`, but plaintext-plaintext division is not allowed. So I'll submit a minimal variant, with the add expressed as `b + x*a` to keep the circuit the same. It is unlikely to win, but it is a valid kernel.

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