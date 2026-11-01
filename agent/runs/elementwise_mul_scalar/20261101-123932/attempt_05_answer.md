I'm submitting the baseline again. The circuit can't be made smaller: computing `x0 * s` takes one ciphertext × plaintext multiply, and CHEHAB adds the rescale itself. Every other circuit would add operations and run slower. The baseline (4.59 ms) is optimal for this task, and further attempts won't change that.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```