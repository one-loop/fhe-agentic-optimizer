The kernel is still a single plaintext encode plus one ciphertext add, which is the minimum for x0 + p. The differences between runs (4.69, 4.68, 4.66 ms) are measurement noise, and no other formulation with the available operators avoids the encoding. I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```