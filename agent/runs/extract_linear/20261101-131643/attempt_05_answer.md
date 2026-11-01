Attempt 4 matched the baseline circuit, as I expected. I still have no cheaper or different correct circuit to propose:

- The baseline uses only the two operations this function needs: one ct-pt multiply (with its automatic rescale) and one rotation.
- Rotating the plaintext mask is rejected (attempt 2).
- Rotating the ciphertext before the multiply gives wrong results for a general mask.
- Any other restructuring only adds operations.

Every remaining attempt would either repeat the baseline circuit, be slower, or be incorrect. I recommend ending the task here with the baseline as the final kernel:

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