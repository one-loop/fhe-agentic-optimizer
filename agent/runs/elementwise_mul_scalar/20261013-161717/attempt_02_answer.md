The kernel is unchanged: one ciphertext-plaintext multiply, the minimum work for `y = x0 * s`. The 9.19 ms → 8.92 ms difference is measurement noise, not a code change. Any alternative I can think of (rotations, extra adds, ciphertext-ciphertext products) adds work, so I'm keeping this kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```