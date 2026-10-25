The attempt-2 failure was a scale mismatch at level 4. The `t6term` product has the pattern (ct·pt)·ct, while `t4*c4` has the pattern (ct·ct)·pt, so the two terms met at the same level with different scales.

Fix: I rewrite the c4 term as 2·(w·c4)·w − c4, using T4 = 2w² − 1. It now has the same (ct·pt)·ct pattern as the c6 term. The final subtraction is a plaintext subtract, which is allowed. This avoids a mismatch and keeps the depth at 4 for the deep terms.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext cw6 = w * c6;
  Ciphertext s = t4 + t4 - one;
  Ciphertext t6term = cw6 * s;
  Ciphertext cw4 = w * c4;
  Ciphertext p4 = cw4 * w;
  Ciphertext t4term = p4 + p4 - c4;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4term;
  result = result + t6term;
  result = result + c0;
  result.set_output("y");
}
```