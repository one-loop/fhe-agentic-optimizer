No cheaper circuit exists for this task, so I'm resubmitting the baseline and recommend not spending the remaining attempts.

- **The baseline is the minimum.** It is a single plaintext add (~2.5 ms of encoding plus ~0.5 ms of add), with zero multiplications, rotations, rescales, mod switches or alignments.
- **The add cannot be removed.** The output depends on both `x0` and `p`, and an add-plain is the only operation that combines a ciphertext with a plaintext vector without consuming a level.
- **Every change costs more.** Any different circuit means extra operations, such as a multiply-plain, a rotation or a ciphertext add, all of which add time. CHEHAB also normalizes rewrites like `(x0 + p) - x0 + x0` back to the same IR, so they don't change the circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```