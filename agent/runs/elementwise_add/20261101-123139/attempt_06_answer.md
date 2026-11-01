I still have no cheaper circuit to offer, because one does not exist. The current circuit is a single ciphertext addition at depth 0, with no multiplications, rotations, rescales or alignments. The output depends on both encrypted inputs, so at least one homomorphic operation is needed. One addition is the floor, and any structurally different circuit adds work and can only be slower.

The optimal kernel is below, unchanged. The last 3 attempts cannot improve on it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```