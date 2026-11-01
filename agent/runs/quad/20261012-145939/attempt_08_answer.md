I'm keeping the same kernel. Attempt 7 measured 31.00 ms, the same noise band as every other run (30.93 to 31.01 ms). The computation is one ciphertext squaring with one relinearization and one rescale at depth 1, the minimum for x². I'm using the last attempt to submit this unchanged, since no alternative formulation can avoid the multiply.

```cpp
// y = x^2: one ciphertext squaring (relinearize + rescale), depth 1.
// This is the minimum possible work for a quadratic activation.

void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```