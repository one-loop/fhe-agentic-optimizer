I don't see a cheaper circuit. The baseline is already a single plaintext multiply plus its rescale, and `s` is known only at run time, so the multiply can't be removed or replaced by additions. Any other structure I can write adds operations, and the circuit-identity rule means reordering won't count.

I'm submitting the operand-swapped form, `s * x0`, which I expect CHEHAB to normalize to the same circuit. It is the minimal correct kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```