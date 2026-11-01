Attempt 1 was the baseline code, so it could not win. This attempt is the rotated-mask variant I described earlier: rotate the ciphertext first, then multiply by the rotated mask. It computes the same function, since `(input * mask) << k == (input << k) * (mask << k)`.

If CHEHAB folds the mask rotation into plaintext encoding, the circuit changes: the ciphertext rotation moves before the multiply. That makes it a new circuit, though probably not a faster one, because the rotation then runs at the higher level. If CHEHAB rejects the plaintext rotation, the error will confirm that this route is closed.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input << offset) * (mask << offset);
  output.set_output("output");
}
```