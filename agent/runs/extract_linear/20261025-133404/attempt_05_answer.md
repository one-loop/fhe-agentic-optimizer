Attempt 4 showed that a rotation is the dominant cost: adding one rotation took the time from about 59 ms to about 105 ms. The baseline already has the minimum, one rotation and one plaintext multiply with its rescale.

I can't find a valid circuit that is cheaper:
- Removing the rotation changes the output.
- Rotating before the multiply needs a rotated plaintext mask, which CKKS rule 2 rejects.
- Making the rotation cheaper would mean lowering the ciphertext's level first without changing its values. That needs a constant-one plaintext, which the rules don't allow.

So the baseline form remains the best I have. I am resubmitting it unchanged.

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