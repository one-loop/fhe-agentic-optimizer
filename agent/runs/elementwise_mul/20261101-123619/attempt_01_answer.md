The baseline is already the minimal circuit for this task, so I'm returning it unchanged.

The output `y = x0 * x1` needs at least one ciphertext-ciphertext multiplication. CHEHAB inserts the relinearization and the one rescale automatically, and there are no rotations, plaintext operations, additions or alignments to remove. One level is the least any product can use.

The alternatives I can see are all slower:
- **Squares:** `((x0+x1)² − (x0−x1)²)/4` needs two multiplications. It also needs a 1/4 constant, which CKKS does not allow here.
- **Masking or rotation:** these add work and save nothing.

Because a winning kernel must have a different circuit, and every different circuit for this function costs more, no kernel can beat the baseline. I expect it to be rejected as unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```