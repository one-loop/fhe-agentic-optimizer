I have no cheaper circuit, and the result is the same as in attempts 1 to 4. The baseline `x0 * x1` has one ciphertext-ciphertext multiply, one relinearization and one rescale, which is the minimum for a product of two ciphertexts. Any different circuit would have to add operations or need constants this task doesn't supply, so it could only be slower or wrong.

I'm not going to spend the remaining three attempts on variants I expect to be worse. The baseline stays the best kernel, and I'm submitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```