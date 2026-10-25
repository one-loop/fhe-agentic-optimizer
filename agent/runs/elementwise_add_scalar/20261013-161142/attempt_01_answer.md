The baseline already does the minimum work: one plaintext add, no multiplications, no rotations, no level consumption. Any alternative would need either a multiplication or a rotation, which cost more, so I'm keeping the same computation and only making the kernel explicit about its structure.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```