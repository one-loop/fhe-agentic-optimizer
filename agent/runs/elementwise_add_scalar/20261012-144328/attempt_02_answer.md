I can't find a faster kernel. The computation is a single ciphertext-plaintext addition with zero multiplications, rotations, or level consumption, and the measured time is 0.26 ms. Any alternative I can think of would add operations, not remove them.

The only remaining variation is the order of operands (`s + x0`). That is the same single addition and would likely time the same, so I'm not submitting it as an improvement. I'm keeping the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```