This final attempt is the baseline kernel again, because no cheaper circuit exists for `y = x0 * s`. It needs exactly one ciphertext × scalar-plaintext multiply, and CHEHAB adds the rescale after it. Any structurally different circuit would add operations and run slower. The baseline (4.59 ms) is the optimal result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```