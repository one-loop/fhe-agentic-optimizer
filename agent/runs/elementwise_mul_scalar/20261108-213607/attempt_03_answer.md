No kernel can beat the baseline on this task. I don't recommend using the remaining 6 attempts, because each one is either identical (not timed) or strictly slower.

`y = x0 * s` with a runtime plaintext `s` needs at least one ct-pt multiply. CKKS also forces one rescale after it, and CHEHAB inserts that automatically. The baseline is exactly that: 1 multiply, 1 rescale, 1 level, and nothing else. A different circuit can only add operations, such as additions, alignments or extra multiplies, so it cannot be at least 3% faster.

The kernel below is the optimal one, unchanged. It is the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```