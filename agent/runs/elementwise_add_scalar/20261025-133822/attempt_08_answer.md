The baseline is a single ciphertext-plaintext add, and I still see no circuit with fewer operations that computes x0 + s. Any structurally different version would add a multiplication, rotation, rescale or alignment, and all of those cost more than the 0.27 ms add. The only variants that avoid extra work compile back to the same circuit, as the last seven attempts showed.

This is my final attempt, so I'm submitting the baseline kernel unchanged. It is correct but cannot be recorded as an improvement.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```