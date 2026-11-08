I still have no valid circuit that is cheaper than the baseline. Resubmitting it a sixth time won't change the verdict, so I'm stating the conclusion rather than pretending to make progress.

The kernel below is the minimal form: one plaintext multiply with its rescale. Anything structurally different adds operations. That would only make it slower, and it could not become the new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```