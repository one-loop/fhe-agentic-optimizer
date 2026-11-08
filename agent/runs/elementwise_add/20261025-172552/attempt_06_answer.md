The baseline is a single ciphertext add, which is already the minimum. The output needs both inputs, so at least one operation is unavoidable. Every equivalent form I've tried compiles to the same circuit, and any circuit that differs from it would have to add operations, so it would be slower. I'm submitting the minimal kernel again rather than a variant that only adds cost.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```