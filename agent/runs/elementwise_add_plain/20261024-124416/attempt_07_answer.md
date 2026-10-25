The kernel is still one plaintext encode and one ciphertext add, which is the minimum for x0 + p. Every attempt produced the same operation counts, and the timing differences (4.64–4.69 ms) are noise. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```