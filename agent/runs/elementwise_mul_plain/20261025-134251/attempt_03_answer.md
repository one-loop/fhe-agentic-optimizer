I have no variant that is cheaper than a single plaintext multiply with one rescale. Anything with a different circuit does extra work, so it is unlikely to beat 7.16 ms. I'm trying one variant anyway to learn whether CHEHAB handles it differently: multiply the ciphertext by the plaintext twice, each time by half of what it needs, is not possible because constants are not allowed. The only legal variant I can see is to compute `x0 * p` as `(x0 + x0) * p` and then subtract `x0 * p`. That gives `x0 * p` with two plaintext multiplies and is strictly more work.

The honest result is that the baseline is probably optimal. I'm resubmitting the minimal kernel instead of a deliberately slower one.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```