Seven runs of the identical program gave 18.06 to 18.54 ms. That spread is measurement noise, so the "best so far" figures don't reflect any real improvement. The kernel needs one plaintext multiply by A, one rescale and one plaintext add of B. I have no equivalent form that removes any of these. Folding B into the multiply would require plaintext-plaintext arithmetic, which is not allowed. For the last attempt I'm keeping the minimal form instead of adding operations that could only slow it down.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```