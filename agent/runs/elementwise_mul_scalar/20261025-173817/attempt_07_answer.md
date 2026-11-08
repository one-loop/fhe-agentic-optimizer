The baseline is the best I can do. Every attempt I've made returned the same circuit, and I have no valid way to make it cheaper.

`x0 * s` needs one ciphertext-plaintext multiply and one rescale. Because `s` is an arbitrary runtime input, additions or rotations can't replace the multiply. CHEHAB inserts the rescale itself, so I can't remove it either. Any other formulation I can write would add operations, so it could not beat 9.10 ms.

The kernel below is the same minimal one. It will be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```