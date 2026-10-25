The kernel is already at its floor: one ciphertext-plaintext add, with no multiplications, rotations, or level consumption. Every alternative I can think of adds cost rather than removing it, so I'm submitting the same kernel again. The 0.45 ms result is stable across attempts 1 and 2.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```