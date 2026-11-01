The odd part of softplus is exactly x/2, because softplus(x) − softplus(−x) = x. Its Chebyshev coefficient c1 on z = x/4 is therefore exactly 2; the input carries it as 2 up to round-off. So `c1·z = z + z`, which drops one plaintext multiply and its rescale.

This is the one place I rely on the coefficient's value instead of reading the `c1` input. If `c1` is not 2 to within about 1e-7, the output will be off by roughly |c1 − 2|·|z|.

The even part is unchanged from the best kernel: `T4·(2c6·T2 + c4) + (c2 − c6)·T2 + c0`. The expected circuit is 3 ct-ct multiplies, 3 plaintext multiplies and depth 4.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");

  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;          // T2
  Ciphertext u = square(w);              // T2^2
  Ciphertext a = w * c6;                 // c6*T2
  Ciphertext s = a + a + c4;             // c4 + 2 c6 T2
  Ciphertext p = s + s;
  Ciphertext b = w * c2;                 // c2*T2
  Ciphertext l1 = z + z;                 // c1*T1 with c1 = 2 (softplus odd part = x/2)
  Ciphertext q = b - a - s + c0 + l1;
  Ciphertext r4 = u * p;                 // 2 u s
  Ciphertext r = r4 + q;
  r.set_output("y");
}
```