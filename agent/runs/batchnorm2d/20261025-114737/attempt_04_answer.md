The 18.63 ms result was noise: the timing against the baseline showed no real gain. The negated circuit also carries two extra additions. I'll try a variant that puts the negations after the multiply, where the ciphertext is at a lower level and operations are cheaper. This also gives a different circuit signature.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext nm = -m;
  Ciphertext y = b - nm;
  y.set_output("y");
}
```