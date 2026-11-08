Idea: compute s² (a square) in parallel with c6·s instead of chaining two multiplies. Then r = s²·(4c6·s + 2c4) + c2·s − 3c6·s. This costs one extra plaintext multiply but cuts the multiplicative depth from 5 to 4, so later ops run at deeper, cheaper levels.

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
  Ciphertext p = square(s);
  Ciphertext w = s * c6;
  Ciphertext w2 = w + w;
  Ciphertext w4 = w2 + w2;
  Ciphertext inner = w4 + c4;
  inner = inner + c4;
  Ciphertext r = p * inner;
  r = r + s * c2;
  r = r - w;
  r = r - w2;
  r = r + z * c1;
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```