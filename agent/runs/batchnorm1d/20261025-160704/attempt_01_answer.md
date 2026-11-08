The baseline does one plaintext multiply, one rescale and one plaintext add (the add costs about 2.5 ms for encoding). The circuit is already minimal: y = x*A + B needs one ct-pt multiply, and B has to enter somehow.

One idea is to avoid the rescale by folding B in differently. For example, `(x + B/A) * A` would need a plaintext division, which isn't allowed, and it would still need the multiply and rescale.

Another idea is to change the order so the add happens at a different level. Adding B after the rescale uses a plaintext at the normal scale, which is what the baseline already does. Adding B before the multiply would need B/A, which can't be computed.

The remaining option is a different circuit structure that CHEHAB might evaluate more cheaply. The plaintext-add encoding cost is unavoidable because B is a plaintext vector input. Multiplying a ciphertext by a plaintext and then adding is the same circuit however I write it. So I'll make one attempt with an explicit subtraction form, `y = x*A - (-B)`. This cannot help, because negating a plaintext is plaintext arithmetic and is not allowed.

I'll resubmit the baseline form. It is likely optimal, and any variant will probably be normalized to the same circuit.

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