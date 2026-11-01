I'm still at the same conclusion. The multiply by the unrotated mask and the single rotation are both required, and nothing in the DSL lets me fuse or remove either one. Moving the rotation before the multiply would need a rotated mask, which is a plaintext-plaintext operation the backend rejects. I'm submitting the baseline circuit again rather than a variant that is slower or incorrect.

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