Idea: write the even part as a polynomial in s = T2(z) = 2z²−1, which gives T4 = 2s²−1 and T6 = 4s³−3s. Then

y = c0 − c4 + c1·z + s·(c2·s⁰ − 3c6 + (2c4 + 4c6·s)·s).

The scalar combinations (4c6, 2c4) are built from ciphertext additions of `c6*s` and plaintext adds, so no plaintext-plaintext arithmetic is needed. This cuts the circuit to 2 squares plus 1 ct-ct multiply, with depth 4.

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
  Ciphertext u = square(z);
  Ciphertext s = u + u - one;
  Ciphertext w = s * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext m = s * inner;
  Ciphertext lin = s * c2;
  Ciphertext r = m + lin;
  r = r - w;
  r = r - w2;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```