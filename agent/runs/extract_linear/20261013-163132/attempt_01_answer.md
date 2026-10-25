The baseline already uses the minimum work for this computation: one plaintext multiply and one rotation, at one level. The rotation is unavoidable because output slot i needs input slot i+offset, and the mask is a plaintext diagonal that must be applied elementwise. Rotating after the multiply keeps the rotation at the already-reduced level, which is cheaper than rotating the fresh ciphertext first. Rotating the mask plaintext instead would not reduce the ciphertext operations, so I keep the baseline structure and make the offset explicit.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // One ct-pt multiply (consumes one level), then one rotation applied at the
  // reduced level after rescale. This is the minimal op count for this kernel.
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
```