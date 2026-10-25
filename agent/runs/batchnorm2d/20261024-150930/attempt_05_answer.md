Attempt 4 was my sign error: -(B - x*A) is x*A - B, not x*A + B. The fix is to negate x before the multiply, so t = (-x)*A = -x*A, and then compute y = B - t = x*A + B.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
```