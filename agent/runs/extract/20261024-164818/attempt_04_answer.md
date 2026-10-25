I don't have a cheaper valid circuit, so this is the baseline kernel unchanged.

The output needs one mask multiply and one rotation, and I found no way to remove or cheapen either:

- **Rotating a plaintext:** attempt 2 showed CHEHAB rejects it as plaintext-plaintext evaluation, so the mask can't be pre-rotated.
- **Rotating first:** the unrotated mask would then be applied at the wrong slots. The same rotation would also run at the top level, where it costs more.
- **Skipping the mask:** slots outside the window would then hold wrapped-around input values instead of zeros.
- **Splitting the rotation:** two smaller rotations would cost more than one.

Multiply then rotate is the cheapest ordering because the rotation runs after the rescale, at a reduced level. Attempt 3 showed that rewriting the rotation direction leaves the circuit identical, so I'm not spending further attempts on rewrites.

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