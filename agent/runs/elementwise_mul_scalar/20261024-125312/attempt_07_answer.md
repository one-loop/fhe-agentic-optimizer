The kernel is still a single ciphertext-plaintext multiply with its rescale, which is the minimum needed for y = x0 * s. The run-to-run spread (8.80 to 9.46 ms) is noise on identical programs. I know of no cheaper way in this DSL to multiply by an arbitrary plaintext scalar, so I'm resubmitting it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```