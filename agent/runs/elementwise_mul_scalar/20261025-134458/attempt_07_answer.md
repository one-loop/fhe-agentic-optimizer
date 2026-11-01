I have no new idea that beats the baseline. The baseline is one scalar ct-pt multiply plus one rescale, with no additions, rotations or alignments. Any other correct form I can write adds operations or collapses back to this circuit. The baseline stays the best at 4.60 ms, and I'm submitting it again only because a kernel is required.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```