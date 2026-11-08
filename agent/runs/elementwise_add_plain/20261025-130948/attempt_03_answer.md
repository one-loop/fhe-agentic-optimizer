The circuit is one ciphertext + plaintext add, and I see no legal way to make it cheaper. A different circuit would need extra operations, which can only cost more. Any rewrite that CHEHAB normalizes back to a single add is rejected as identical to the baseline. Here I resubmit the same kernel, and I expect it to be rejected as identical again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```