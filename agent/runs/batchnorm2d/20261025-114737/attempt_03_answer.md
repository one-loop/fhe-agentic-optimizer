My last reply had two code blocks, which broke the format. This time there is only one.

The baseline is already at the minimum for this function: one plaintext multiply with its rescale, and one plaintext add. The kernel needs a different circuit to count, so I'm trying a form that uses a negated ciphertext and a subtraction. CHEHAB may lower it differently, for example with the sign folded into the plaintext encoding. I don't expect a large gain, and it could be slightly slower, because a negation is cheap but not free.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext m = nx * a;
  Ciphertext y = b - m;
  y.set_output("y");
}
```