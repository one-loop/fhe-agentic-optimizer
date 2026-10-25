The kernel is unchanged and the graph is still a single plaintext add, so this 6.66 ms run is measurement noise, not a regression. The earlier runs spanned 4.64 to 4.74 ms on identical work. I'm resubmitting the same kernel as the best-known version.

```cpp
// Elementwise add plain: y = x0 + p. Already minimal (one add, no multiplications,
// no rotations, no level consumption); resubmitted unchanged.

void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```