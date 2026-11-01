The kernel is still at the floor: one plaintext add, with no multiplications, rotations, or level use. Swapping operand order (`p + x0`) would not change the encoding cost, so I'm resubmitting the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```