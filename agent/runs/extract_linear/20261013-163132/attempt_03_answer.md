The failure came from rotating the plaintext mask: `mask << offset` is a plaintext-only expression, which the backend cannot evaluate. The fix is to keep every rotation on a ciphertext, which means reverting to the baseline ordering (multiply, then rotate). That ordering is already the best measured so far at 59.11 ms, so I'm returning to it.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Multiply by the mask first, then rotate the (already reduced-level) product.
  // Plaintext rotations are not allowed, so the rotation stays on the ciphertext.
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```