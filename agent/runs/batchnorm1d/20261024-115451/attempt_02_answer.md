The kernel is already at the minimum operation count: one ct-pt multiply and one plaintext add. I can't find a cheaper algebraic form, so I'm trying a variation in how the operations are written. Here the add is written as an explicit `+=` on a copy of the product. CHEHAB should compile this to the same graph, so I expect little or no change in time.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```