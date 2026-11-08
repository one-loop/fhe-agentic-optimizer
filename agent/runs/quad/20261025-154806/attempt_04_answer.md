I have no cheaper circuit to propose. The rest of this answer repeats the minimal kernel; it should not be counted as a new attempt.

Squaring needs one ciphertext multiplication, which comes with one relinearization and one rescale. The task has no plaintext inputs, so CKKS has no constants to rescale with, and I can't emit mod switches or level changes myself. Every other formulation I can build, such as splitting into sums, using rotations, or multiplying by `x+x`, adds operations or needs a division by a constant. All of those would be slower or wrong.

`x * x` and `square(x)` both reduce to this one multiplication, and the measured difference between them (63.6 ms vs 64.5 ms) is within noise. The baseline is already the optimal circuit for this task.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```