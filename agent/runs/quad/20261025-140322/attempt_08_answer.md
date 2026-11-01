Attempt 7 was slower again (32.90 ms), as the added adds predicted. I have not found any circuit cheaper than the baseline. One ct-ct multiply with its relinearize and rescale is the minimum for x², and `square(x)` gave only a ~1% gain, which is below the 3% threshold.

For the last attempt I'm submitting the `square(x)` form again, which is the fastest circuit measured and the best candidate if timing noise favours it. CHEHAB will report it as identical to attempt 1, so it will not change the best result.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```