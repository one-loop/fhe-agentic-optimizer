The `square(x)` attempt came out at 67.50 ms against 64.64 ms for the baseline. That difference is most likely measurement noise, since both versions compile to one multiply, one relinearization and one rescale. The minimum operation count for x² is already reached, so there is little left to remove.

I considered rewriting x² as ((x+x)² - ... ) or splitting it into other forms, but each of those adds multiplications or plaintext operations and can't beat one squaring. Without plaintext constants or extra inputs, no cheaper algebraic route exists. I'm going back to the baseline form, which is the best result so far.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```