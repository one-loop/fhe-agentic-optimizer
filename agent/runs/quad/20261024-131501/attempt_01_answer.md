The baseline is already a single ciphertext-ciphertext multiply, which is the minimum for computing x² on encrypted data. `square(c)` expresses the same operation directly and may lower to a cheaper squaring path, since it can skip some of the work a general multiply does. I'll use that.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```