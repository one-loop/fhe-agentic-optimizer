The baseline is already close to minimal: one ct-pt multiply with rescale, plus one plaintext add. The plaintext add costs about 2.5 ms for encoding, and the multiply costs about 7 ms. The measured 18 ms probably includes the encoding of A and B.

I don't see a cheaper algebraic form, since each operand has to be used at least once. The only change I can try is the order of operations. Computing `(x + b/a) * a` would need a plaintext division, which isn't allowed. So I keep the same computation and make only a small structural change: the add happens after the rescale, at the deeper level, which is cheaper. That is already what the baseline does.

I'm submitting the baseline form unchanged, as there is no clear improvement to make.

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