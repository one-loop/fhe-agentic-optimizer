The idea is to build `T3` as `z·(2a−1)`, with `b = a + a − one` formed first. That removes the ciphertext subtraction `zt2 + zt2 − z`, which needed `z` aligned down to a lower level. The rest of the kernel is attempt 2's structure.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext b = t2 + t2 - one;
  Ciphertext t3 = z * b;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext D0 = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext D = D0 + D0 + c4;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext sub = t3 * c5 + t2 * c6 + z * c7;
  Ciphertext r = (low - sub) + t4 * D;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```