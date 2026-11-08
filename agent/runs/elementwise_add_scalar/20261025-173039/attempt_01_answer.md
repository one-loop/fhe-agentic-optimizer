The baseline is already a single ciphertext-plaintext add with no multiplications, rotations or rescales. The only cost is the add itself (about 0.5 ms), which includes encoding the scalar plaintext.

I can't remove that add. Any variant, such as `s + x0` or an add via subtraction, would produce the same circuit, and CHEHAB would normalize it away. Adding extra operations would only slow it down.

I'm submitting the baseline form unchanged, since it is already the minimal circuit. It will not count as a new best, because its circuit matches the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```