I don't have a cheaper circuit. x² needs exactly one ciphertext multiplication with its relinearization and rescale, and no numeric constants or extra plaintext inputs are available to build a cheaper identity. Rotations, plaintext multiplications and alignments would only add cost.

The remaining option is a structural variant with the same multiplication count. It negates only one operand, so it uses one fewer negation than attempt 2. It is expected to land within noise of the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = -(x * (-x));
  y.set_output("y");
}
```