The baseline is already a single ciphertext-plaintext multiply plus one rescale, which is the minimum needed to compute x0 * p. The function is inherently one multiplication by an arbitrary plaintext vector, so I cannot remove it.

I can't build a cheaper equivalent circuit. Rotations, extra additions and splitting the multiply would all add cost. I'm therefore submitting the same minimal kernel. It will likely not count as a new best because its circuit is identical to the baseline's.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```