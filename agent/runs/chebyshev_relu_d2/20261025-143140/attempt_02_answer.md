The best kernel has one ct-ct multiplication and two ct-pt multiplications. I could not find a form with fewer multiplications, because the combined coefficients such as c1·α and 2·c2·α² would need plaintext-plaintext arithmetic.

This attempt tries to lower the cost of the additions instead. I put the plaintext add of c1 on the shallower side and merge the doubling into the scaling by multiplying z by c2 first, then doubling. The final constants are added at the deepest level, where each operation is cheapest.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext u = (w + w) + c1;
  Ciphertext p = u * z;
  Ciphertext y = (p - c2) + c0;
  y.set_output("y");
}
```