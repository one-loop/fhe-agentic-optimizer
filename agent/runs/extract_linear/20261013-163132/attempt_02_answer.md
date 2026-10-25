The last attempt performed the same operations as the baseline, and its 60 ms is within noise of the 59 ms baseline. To test a different structure, this attempt rotates the input first and multiplies by the pre-rotated mask. That moves the rotation to the fresh ciphertext and the multiply to the rotated ciphertext, so the op count is unchanged but the cost distribution differs. It may be slower, since the rotation then runs at the full level.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  // Alternative ordering: rotate the input first, then multiply by the
  // rotated mask so that the product lands in the same slots as before.
  Ciphertext rotated = input << offset;
  Ciphertext output = rotated * (mask << offset);
  output.set_output("output");
}
```