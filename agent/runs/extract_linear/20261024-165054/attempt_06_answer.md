I still have no cheaper circuit to propose, and I'm not going to submit another kernel that repeats the baseline circuit.

Attempts 2 to 5 all compiled to the same circuit as the baseline. Rewrites that keep the same operations get normalized away and can't win.

The baseline is already at the minimum of one ct-pt multiply with rescale and one rotation:

- **Pre-rotating the mask:** this would let me rotate first, but a plaintext-plaintext operation is rejected, as attempt 1 showed.
- **Splitting the rotation or adding steps:** these only add operations.

The best kernel remains the baseline at 59.04 ms.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```