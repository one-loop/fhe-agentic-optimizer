This variant computes r = c3·w + c5·(w² − 5w) directly. It still has 3 ciphertext multiplications (u, w·w, z·r), but the plaintext multiplications by c3 and c5 now happen at different levels.

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
  Ciphertext r = w * c3 + q * c5;
  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc3x3 = (zc3 + zc3) + zc3;
  Ciphertext zc5x2 = zc5 + zc5;
  Ciphertext zc5x5 = (zc5x2 + zc5x2) + zc5;
  Ciphertext K = (zc1 - zc3x3) + zc5x5;
  Ciphertext y = K + z * r;
  y = y + c0;
  y.set_output("y");
}
```