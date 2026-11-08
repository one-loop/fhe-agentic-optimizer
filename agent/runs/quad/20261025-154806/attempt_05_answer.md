I have no valid, cheaper circuit left to try, and I won't submit another variant that CHEHAB reduces to the same circuit.

Any correct kernel needs at least one ciphertext multiplication with its relinearize and rescale. There are no plaintext inputs to supply constants, and I can't emit mod switches myself. The alternatives I can build all fall into one of two groups:
- **Extra work:** extra multiplications, rotations, or adds, which are strictly slower.
- **Wrong function:** results like 2x² or 4x² that would need a division by a constant, which CKKS can't express here.

The baseline `x * x`, a single multiplication at depth 1, is therefore the best circuit I can offer. It remains the kernel to keep.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```