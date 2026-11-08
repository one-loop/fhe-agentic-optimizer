I have no cheaper structure left to try. Every identity I can express without numeric constants or plaintext-plaintext arithmetic needs the same plaintext multiply with its rescale, plus a plaintext add. The negation variants only add cheap operations on top of that. Attempt 3's apparent speed-up was not confirmed in the alternating batches, so I'm treating it as noise. I'm resubmitting the minimal kernel.

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