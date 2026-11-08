I don't have a cheaper circuit that computes the same function. The attempt-2 result confirms that extra rotations only add cost: 105 ms against 59 ms.

- **Plaintext multiply:** it is needed to apply the mask, and it cannot be merged with the rotation. The mask cannot be pre-rotated without plaintext-plaintext arithmetic, which CKKS here forbids.
- **Rotation:** it is needed to move the window. It is cheapest after the multiply, because the ciphertext then has fewer levels left.

The baseline is therefore the minimal circuit, and I'm resubmitting it unchanged.

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