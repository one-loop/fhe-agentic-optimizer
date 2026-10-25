This version computes y = -(B - x*A), which equals x*A + B. It uses one negation instead of two, so it is a smaller circuit than attempt 3. It still probably won't beat the baseline.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext u = b - t;
  Ciphertext y = -u;
  y.set_output("y");
}
```