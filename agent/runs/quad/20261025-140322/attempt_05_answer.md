Attempt 4 matched the baseline within noise, as expected: the two extra adds cost almost nothing, and the multiply, relinearize and rescale still dominate. I still have no cheaper circuit, because a single ct-ct multiply is the minimum for x².

This attempt changes the form to a mixed product, `x * ((x + x) - x)`. It is still x², but it takes the plain multiply path instead of `square`, with the extra adds as in attempt 4. It is a different circuit from every earlier attempt, but I don't expect it to beat the baseline by 3%.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = (x + x) - x;
  Ciphertext y = x * t;
  y.set_output("y");
}
```