I could not find a cheaper circuit, so the baseline stays as the best kernel. Attempts 2, 3, 5, 6 and 7 were rewrites of the same computation that CHEHAB reduced to the baseline circuit. Attempt 4 split the rotation in two and was slower, at 104.95 ms against 59.15 ms.

- **Rotation:** it is the dominant cost and the output needs exactly one shift, so it cannot be removed.
- **Multiply:** the mask must be applied, so one plaintext multiply with its rescale is required.
- **Rotating before the multiply:** this needs a rotated plaintext mask, which CKKS rule 2 rejects (attempt 1).
- **Making the rotation cheaper:** this would need a level drop that leaves the values unchanged, which requires a constant-one plaintext that the rules do not allow.

For the last attempt I am submitting the same baseline form.

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