This is the same kernel as before; it cannot beat the baseline, and I'm not offering a slower variant just to change the circuit.

The function `y = x0 * s` needs one ciphertext × plaintext multiply, and CHEHAB adds the rescale itself. That baseline has no adds, rotations, relinearizations or alignments to remove. Any structurally different circuit would add operations, so it would be slower and could never be confirmed faster.

I suggest ending the task with the baseline (4.59 ms) as the final result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```