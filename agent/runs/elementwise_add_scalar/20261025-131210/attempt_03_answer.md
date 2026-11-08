The failure is explained by the circuit. `x0 - (x0 - s)` makes the ciphertext part cancel to zero, so SEAL reports a transparent ciphertext. That was a poor idea, and I should not have expected it to be faster.

The baseline is a single add with one plaintext encoding, and I don't see a valid circuit with fewer operations. The one other option is to change how the scalar is added, without cancelling the ciphertext:

- `y = s + x0` is normalized to the same circuit as the baseline.
- `y = x0 - (-s)` needs plaintext negation, which is not allowed.
- Subtracting a negated ciphertext, `x0 - (-x0 - s)`, doubles x0 and does not give x0 + s.

I'm trying `(x0 + x0) + s` minus `x0`. It computes 2·x0 + s − x0 = x0 + s, and it never cancels to a transparent ciphertext. It has three adds instead of one, so I expect it to be slower. It is a different circuit that stays valid, and it shows whether the measurement can differ at all.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext d = x0 + x0;
  Ciphertext t = d + s;
  Ciphertext y = t - x0;
  y.set_output("y");
}
```