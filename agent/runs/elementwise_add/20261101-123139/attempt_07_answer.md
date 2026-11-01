No cheaper circuit exists for this task. The baseline uses exactly one ciphertext addition and nothing else. Because `y` depends on both encrypted inputs, at least one homomorphic operation is required, and a single add is the minimum. Any structurally different circuit must contain extra operations and would be slower.

I'm returning the optimal kernel again rather than a deliberately worse variant.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```