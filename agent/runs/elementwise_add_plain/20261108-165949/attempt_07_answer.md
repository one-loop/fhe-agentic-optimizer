I still have no cheaper circuit. The baseline is one plaintext add, and every alternative I can construct needs at least that add plus extra operations, so none could beat it. Six attempts have produced the identical circuit. The baseline remains the best kernel, and I'm submitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```