The baseline is probably already at the minimum for this function: one plaintext multiply with its rescale, plus one add. Every other equivalent form I can write needs plaintext-plaintext arithmetic (such as B/A or A·B), which CKKS rules forbid, or it adds operations.

I'm resubmitting the baseline circuit, in the lowest-cost form I can find. I expect it to be flagged as identical to the baseline again, and I don't see a legal rewrite that is cheaper.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```