I have no new idea that preserves correctness. I'm resubmitting the baseline structure, since every correct rearrangement I can build under the CKKS constraints gives the same circuit or a larger one.

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