The baseline is already minimal, so I'm returning it unchanged. It won't count as a new best, since the judging rules require a different circuit.

The output `y = x0 * x1` is a product of two encrypted inputs, so it needs at least one ciphertext × ciphertext multiplication. CHEHAB adds the one relinearization and one rescale itself, and the baseline has nothing else: no rotations, plaintext operations, alignments or additions.

The other rewrites I can see are all slower:
- **Squaring identity:** `((x0+x1)² − (x0−x1)²)/4` needs two squares, extra additions and a plaintext constant for the 1/4.
- **Splitting or masking the inputs:** this only adds operations.

Unless the evaluator can find savings outside the kernel, I don't think any kernel can beat 6.12 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```