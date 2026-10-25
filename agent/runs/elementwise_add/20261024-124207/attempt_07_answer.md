Attempt 6 measured 1.02 ms with the same single-add program, so the spread across runs (0.72 to 1.71 ms) is still timing noise. The kernel is one ciphertext add, which is the minimum for x0 + x1. There is no multiplication, rotation, or level change to remove, so I'm submitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```