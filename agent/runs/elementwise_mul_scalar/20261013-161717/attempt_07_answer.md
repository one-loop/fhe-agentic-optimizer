This run came in at 8.90 ms, a hair under the previous best, but the kernel is unchanged and the difference is noise. It already does the minimum work for `y = x0 * s`: one ciphertext-plaintext multiply and one rescale, with no rotations, relinearization, or level alignment. I'm submitting it as the final kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```