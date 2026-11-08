`square(x)` came out about 2% faster than the baseline, which is short of the 3% threshold. That is probably the limit for this task.

`y = x * x` needs exactly one ciphertext multiplication. CHEHAB inserts the one relinearization and one rescale automatically, and there are no rotations, plaintext operations or alignments left to remove. Every other equivalent circuit adds work. For example, an extra multiplication through a polynomial identity, or a plaintext multiply, would also cost a level. A cheaper circuit would need to skip the relinearization or the rescale, and the DSL gives no control over either.

The remaining attempts could only resubmit the same circuit or try a slower one, so neither can win. I'm submitting the `square(x)` kernel again as the best available circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```