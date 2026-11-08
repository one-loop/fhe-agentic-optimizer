I don't see a cheaper circuit for this kernel. `x0 * s` needs exactly one ciphertext-plaintext multiply plus its rescale, and `s` is an arbitrary input, so it can't be replaced by additions or rotations. Every alternative I can write adds operations (extra adds, a second multiply, or alignment steps) or gets collapsed back to the baseline by CHEHAB's CSE and simplification. I can't remove the rescale or the mod-switch myself, because CHEHAB inserts them automatically. Below is the minimal kernel again. It will probably be judged identical to the baseline, so the baseline remains the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```