The circuit has to change. This version negates the product, subtracts B, and negates again, so the result is still x*A + B. The extra negations are cheap, but CHEHAB may handle the plaintext subtraction differently from the plaintext add. It is a long shot, and it may not be faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext u = t - b;
  Ciphertext y = -u;
  y.set_output("y");
}
```