The kernel is unchanged: one plaintext encode and one ciphertext add, which is the minimum for x0 + p. Timings across attempts (4.64–4.69 ms) differ only by noise. I'm resubmitting the best version.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```