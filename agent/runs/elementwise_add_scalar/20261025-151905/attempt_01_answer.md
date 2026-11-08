The baseline is already a single plaintext add, which is the minimum for this function: no multiplications, rotations, rescales or alignments. Any alternative circuit would need at least as many operations. I'm resubmitting it as is, since a rewrite would only add work.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```