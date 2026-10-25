The kernel remains a single ciphertext-plaintext multiply with its rescale, the minimum for y = x0 * s. Every run of this program has landed between 8.80 and 9.46 ms, which is noise, and I haven't found a cheaper equivalent in the DSL. I'm submitting it again for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```