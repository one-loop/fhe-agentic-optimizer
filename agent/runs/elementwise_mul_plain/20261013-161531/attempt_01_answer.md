The baseline already uses the minimum work for this task: one ciphertext-plaintext multiply, one level, no rotations or ciphertext-ciphertext products. Any restructuring would add operations, so I'm submitting the same kernel unchanged.

```cpp
// Elementwise multiply by a plaintext vector: the minimal form (one ct x pt multiply, depth 1).
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```