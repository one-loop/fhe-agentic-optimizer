I can't find a cheaper circuit, so I'm not going to submit another variant just to use up an attempt. Every structurally different form of `x*x` that I can build adds work: extra ciphertext multiplications, additions or rotations. The current circuit has one multiplication, one relinearization, one rescale and one level, which is the minimum for a squaring in CKKS without plaintext constants.

The best kernel is still the `square(x)` form from attempt 1. It was correct, and about 2% faster than the baseline, but that is under the 3% confirmation threshold.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```