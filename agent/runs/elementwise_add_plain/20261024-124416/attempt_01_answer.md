The baseline is a single plaintext add, which is already the minimum for this function. The cost (~4.7 ms) is mostly the plaintext encoding plus one ciphertext add. Without numeric constants or plaintext-only arithmetic, there is no cheaper formulation: any alternative would need the same encoding of `p` and at least one add. I'm keeping the kernel as is.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```