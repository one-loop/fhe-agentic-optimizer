The best result so far is still the baseline (358.63 ms for the last attempt against 357.52 ms). For the last attempt I'm switching to Clenshaw evaluation of the Chebyshev series. The recurrence is b_k = c_k + 2·z·b_{k+1} − b_{k+2}, finishing with y = c0 + z·b1 − b2.

- **Multiplications:** this needs only 6 ciphertext–ciphertext multiplies (z·b6 … z·b1, plus the plaintext multiply z·c7) and none of the 8 coefficient multiplies on T_k.
- **Scale safety:** each b_k combines a product at the newest level with a ciphertext two levels lower and a plaintext. Nothing at the same level is added, so the scale mismatches I hit earlier should not occur.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext zc7 = z * c7;
  Ciphertext b6 = zc7 + zc7 + c6;

  Ciphertext p5 = z * b6;
  Ciphertext b5 = p5 + p5 + c5 - c7;

  Ciphertext p4 = z * b5;
  Ciphertext b4 = p4 + p4 + c4 - b6;

  Ciphertext p3 = z * b4;
  Ciphertext b3 = p3 + p3 + c3 - b5;

  Ciphertext p2 = z * b3;
  Ciphertext b2 = p2 + p2 + c2 - b4;

  Ciphertext p1 = z * b2;
  Ciphertext b1 = p1 + p1 + c1 - b3;

  Ciphertext p0 = z * b1;
  Ciphertext y = p0 + c0 - b2;
  y.set_output("y");
}
```