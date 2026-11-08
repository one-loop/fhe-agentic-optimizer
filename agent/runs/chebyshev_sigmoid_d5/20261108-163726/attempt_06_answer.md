The best kernel (attempt 1) and the kernels based on it all measure about 57–58 ms. So I'm combining the trims that cut additions without adding any ciphertext-ciphertext multiplications:

- Keep attempt 1's structure with w and the 4× applied to m.
- Take the `t = v − 5u` sharing from attempt 3, which gives `low = s − 2v − t`.

I haven't run it, so the timing and accuracy are untested.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext u = z * c5;
  Ciphertext v = z * c3;
  Ciphertext s = z * c1;

  Ciphertext uw = u * w;
  Ciphertext uw2 = uw + uw;
  Ciphertext uw4 = uw2 + uw2;
  Ciphertext u2 = u + u;
  Ciphertext u4 = u2 + u2;
  Ciphertext u5 = u4 + u;
  Ciphertext t = v - u5;
  Ciphertext r = t + uw4;

  Ciphertext m = w * r;
  Ciphertext m2 = m + m;
  Ciphertext m4 = m2 + m2;

  Ciphertext v2 = v + v;
  Ciphertext low = (s - v2) - t;

  Ciphertext result = low + m4;
  result = result + c0;
  result.set_output("y");
}
```