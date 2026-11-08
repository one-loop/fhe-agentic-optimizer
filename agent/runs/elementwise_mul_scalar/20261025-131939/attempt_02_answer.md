My previous message said I was submitting the commuted form `s * x0`, but the code I gave was the unchanged baseline. That is why it was reported as identical.

I don't see a cheaper circuit. The output needs the encrypted `x0` scaled by a value that exists only as a plaintext input. That takes one plaintext multiply and its rescale, and every alternative I can build (such as `(x0+x0)*s - x0*s`) adds operations. The kernel below is the commuted form, which I expect CHEHAB to normalize to the same circuit as the baseline, so it probably can't win.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = s * x0;
  y.set_output("y");
}
```