The kernel is still one plaintext encode and one ciphertext add, the minimum for x0 + p. The remaining differences between runs (4.69, 4.68, 4.66, 4.64 ms) are measurement noise, not real gains. I can't see a formulation with the available operators that avoids the encoding, so I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```