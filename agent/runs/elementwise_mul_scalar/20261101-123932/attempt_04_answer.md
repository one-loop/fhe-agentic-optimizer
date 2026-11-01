I'm submitting the baseline kernel again because no cheaper circuit exists for `y = x0 * s`. It is one ciphertext × plaintext multiply plus the rescale CHEHAB adds itself, with nothing else to remove. Every other circuit adds operations and would be slower. The remaining attempts can't produce a confirmed improvement, so the baseline (4.59 ms) is the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```