The baseline is still my answer, and this last attempt is the same kernel again. I have no cheaper circuit.

`x0 * s` needs one ciphertext-plaintext multiply and one rescale. Because `s` is an arbitrary runtime input, additions or rotations can't replace the multiply. CHEHAB inserts the rescale itself, so I can't remove it. Any rewrite I could write either collapses back to this circuit, as attempts 1–7 did, or adds operations and runs slower. The baseline at 9.10 ms stays the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```