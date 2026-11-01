Attempt 2 was slower mainly because it needed 5 scale alignments. This attempt multiplies the plaintext coefficients into z early, at a shallow level, and uses one more ciphertext multiplication. That brings the depth down to 4, so later operations run at deeper, cheaper levels.

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
  Ciphertext u = z * z;
  Ciphertext u2 = u + u;
  Ciphertext w = u2 + u2;
  Ciphertext ww = w * w;
  Ciphertext w2 = w + w;
  Ciphertext w5 = (w2 + w2) + w;
  Ciphertext q = ww - w5;
  Ciphertext za = z * c3;
  Ciphertext zb = z * c5;
  Ciphertext zc = z * c1;
  Ciphertext zb2 = zb + zb;
  Ciphertext zb5 = (zb2 + zb2) + zb;
  Ciphertext za2 = za + za;
  Ciphertext za3 = za2 + za;
  Ciphertext lin = (zc + zb5) - za3;
  Ciphertext y = (za * w + zb * q) + lin;
  y = y + c0;
  y.set_output("y");
}
```