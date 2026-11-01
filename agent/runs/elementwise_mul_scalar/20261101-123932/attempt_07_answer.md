I'm submitting the baseline again because the circuit can't get smaller. It is a single ciphertext × scalar-plaintext multiply followed by the rescale CHEHAB adds automatically. Any different circuit would only add operations and run slower, so the baseline (4.59 ms) stays the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```