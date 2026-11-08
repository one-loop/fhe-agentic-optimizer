The baseline is already near the minimum for this function. It needs one plaintext multiply with its rescale, because A varies per slot, and one plaintext add for B. I can't see a circuit with fewer operations: folding B in before the multiply would need a plaintext division, and the DSL forbids plaintext-plaintext arithmetic.

This attempt is a structural variant. It negates x, multiplies by A, subtracts B, then negates back. That adds negations, so it is probably not faster. It does give a different circuit, which tells us whether CHEHAB handles the sign differently.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  t -= b;
  Ciphertext y = -t;
  y.set_output("y");
}
```