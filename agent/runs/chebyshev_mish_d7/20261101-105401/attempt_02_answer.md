The previous attempt failed because T4 = T2·T2 has a different multiplication history from T3 = z·T2. Both reach level 4, so c4·T4 cannot be added to c3·T3 or c5·T3 there.

The fix moves c4·T4 into the high part. It is added to 2·T4·q, which sits one level deeper (level 5), so CHEHAB aligns it for free. The low part then joins the high part at level 5.

The circuit is otherwise unchanged:
- **Ciphertext multiplications:** 4 instead of 6.
- **Scalar plaintext multiplications:** 10 instead of 8.
- **Depth:** about 5 instead of 8.

This has not been run yet; it depends on CHEHAB keeping this grouping of the sums rather than reordering them.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext T2 = zz + zz - one;
  Ciphertext zT2 = z * T2;
  Ciphertext T3 = zT2 + zT2 - z;
  Ciphertext T2s = T2 * T2;
  Ciphertext T4 = T2s + T2s - one;

  Ciphertext c6T2 = T2 * c6;
  Ciphertext q = z * c5 + c6T2 + T3 * c7;
  Ciphertext p = T4 * q;
  Ciphertext high = (p + p) + T4 * c4;

  Ciphertext low = (z * c1 - z * c7) + (T2 * c2 - c6T2) + (T3 * c3 - T3 * c5);
  Ciphertext result = high + low + c0;
  result.set_output("y");
}
```