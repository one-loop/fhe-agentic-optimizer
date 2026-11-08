The best circuit has 4 ct-ct multiplications, which looks near the minimum for a degree-7 polynomial, so this attempt only trims additions. I factor the shared factor 2 out of `lin` and `4q`, so the final sum is p = 2·(d + 2q) + e0. That removes one ciphertext add and keeps the same multiplications and depth.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = z * z;
  Ciphertext t = zz + zz - one;
  Ciphertext t2 = t * t;
  Ciphertext s0 = t * c7;
  Ciphertext s = s0 + s0;
  s = s + c5 - c7;
  Ciphertext q = s * t2;
  Ciphertext tc3 = t * c3;
  Ciphertext tc5 = t * c5;
  Ciphertext d = tc3 - tc5 - s0 - s0;
  Ciphertext d2 = d + q + q;
  Ciphertext p = d2 + d2;
  p = p + c1 - c3 - c5 + c7;
  Ciphertext y = z * p;
  y.set_output("y");
}
```

Wait, this is mathematically wrong: 2·(d + 2q) = 2d + 4q requires q = s·t2 with s = 2s0 + c5 − c7, which is what I wrote, so it matches the previous `4·s·t2`. The kernel above is therefore correct.