The kernel needs one ciphertext-plaintext multiply (the mask) and one rotation (the window shift). Both are required, so the operation count can't drop. The only remaining lever is order. Here the rotation runs after the multiply and rescale, so it operates at the already-reduced level, which is the cheapest place for it. Swapping the order would move the ~20 ms rotation to the top level, so I keep the baseline order. I'm also avoiding rotating the plaintext mask, since that would need a plaintext-side operation that isn't free.

```cpp
// Minimal op count: one ct-pt multiply by the mask, then one rotation.
// The rotation stays after the multiply so it runs at the already-reduced level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext masked = input * mask;
  Ciphertext output = masked << offset;
  output.set_output("output");
}
```