Idea: this is attempt 5's structure with the final accumulation reordered. I'm adding the `c1·z` term before the plaintext constants, so the cheap low-level terms are combined early. The circuit is essentially the same: 3 ciphertext-ciphertext multiplications, 3 plaintext multiplications and depth 5.

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
  Ciphertext w = u + u - one;
  Ciphertext e = w * c6;
  Ciphertext e2 = e + e;
  Ciphertext f1 = e2 + c4;
  Ciphertext f = f1 + f1;
  Ciphertext g = w * f + c2;
  Ciphertext e3 = e2 + e;
  Ciphertext lin = z * c1 - e3;
  Ciphertext p = w * g + lin;
  p = p - c4;
  p = p + c0;
  p.set_output("y");
}
```