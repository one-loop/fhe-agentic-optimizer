The baseline is already the minimal circuit. It uses one plaintext multiply and its rescale, with no rotations, ciphertext multiplications or alignments. Any other way to compute `x0 * s` needs the same multiply and adds operations around it. Constants are not allowed, so I can't halve or rescale anything at compile time. I also can't fold `s` into the ciphertext without extra work, because plaintext-plaintext arithmetic is forbidden. I don't see a cheaper valid circuit, so I'm resubmitting the minimal form.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```