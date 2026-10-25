The identical attempt-4 kernel measured 217 ms this time against 201.5 ms before, so run-to-run noise is around 15 ms. I see no further structural saving, so I'm submitting the attempt-4 kernel again.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t = u + u - one;
  Ciphertext w = t * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4 + c4;
  Ciphertext m = t * inner;
  Ciphertext g = m + c2 - c6 - c6 - c6;
  Ciphertext r = t * g + z * c1;
  Ciphertext result = r + c0 - c4;
  result.set_output("y");
}
```