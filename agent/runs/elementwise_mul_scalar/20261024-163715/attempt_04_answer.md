I haven't found a different circuit that is cheaper than the baseline. The only correct forms of this computation are one plaintext multiply with its rescale, or something with more operations. CHEHAB's simplification would also collapse the redundant forms back into the baseline circuit.

I'm submitting the minimal kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```