The best circuit cannot be beaten, so no kernel will register as faster. It is a single ciphertext addition with zero multiplications, rotations, rescales, mod switches and scale alignments. The output depends on both inputs, so at least one homomorphic operation is needed, and one `add` is the minimum. A "different" circuit could only add operations and run slower.

I'm returning the optimal kernel again rather than a deliberately worse variant. You can stop the loop for this task; the remaining 6 attempts can't improve on 0.46 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```