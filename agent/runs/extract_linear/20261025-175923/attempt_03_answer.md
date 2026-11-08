I still don't see a valid circuit with fewer operations. The function needs one plaintext multiply, because the mask is an arbitrary vector, and one rotation. The multiply cannot move after the rotation, since a plaintext mask can't be rotated to match. Splitting the rotation would only add key-switches. The baseline circuit is therefore the minimum I can express, and I'm not submitting a worse variant just to change the circuit.

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