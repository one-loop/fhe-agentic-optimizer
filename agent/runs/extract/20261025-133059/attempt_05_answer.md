I have no correct circuit left that is cheaper than the baseline. Any alternative either needs a rotated plaintext mask, which the backend rejects, or adds more rotations or multiplies. The baseline's single plaintext multiply followed by a single rotation is the minimum, so I'm submitting it again.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  Ciphertext output = (input * mask) << static_cast<int>(slots() / 4);
  output.set_output("output");
}
```