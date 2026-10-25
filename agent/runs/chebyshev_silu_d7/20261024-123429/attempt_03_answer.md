Idea: keep the same algebra but reorder the c6 product. Computing (c6·t)·t instead of c6·(t·t) takes the ct-ct multiply off the t² critical path, so it runs one level shallower and the following operations are cheaper.

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
  Ciphertext b = w * t;
  Ciphertext a = t * c4;
  Ciphertext b2 = b + b;
  Ciphertext b4 = b2 + b2;
  Ciphertext g = b4 + a + a + c2 - c6 - c6 - c6;
  Ciphertext r = t * g + z * c1;
  Ciphertext result = r + c0 - c4;
  result.set_output("y");
}
```