The baseline already uses a single ciphertext-ciphertext multiply, which is the minimum for x*x. The only remaining lever I can see is the operation itself. `square(x)` may lower to a dedicated squaring path (one tensor product instead of the general two-operand multiply, with the same single relinearize and rescale), so I'll try that. It has the same depth and no extra rotations or plaintext work.

```cpp
// y = x * x using the dedicated squaring operator (single ct-ct square, one relin + rescale).

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```