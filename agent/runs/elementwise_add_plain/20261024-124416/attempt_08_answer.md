The kernel is unchanged: one plaintext encode and one ciphertext add, which is the minimum for x0 + p. The timings across attempts (4.64–4.69 ms) differ only by noise, and I have no cheaper formulation to try. I'm submitting the same kernel for the last attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```