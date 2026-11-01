Idea: keep the attempt-2 structure (3 ciphertext-ciphertext multiplications, depth 5) and remove one plaintext subtraction. Since 4w² − 3 = 2·(2w² − 1) − 1, I reuse t = 2w² − 1 and subtract `one` twice instead of three times. Each plaintext add costs about 2.5 ms for encoding.

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
  Ciphertext s = w * w;
  Ciphertext t = s + s - one;
  Ciphertext q = t + t - one;
  Ciphertext b = q * c6;
  Ciphertext a = w * c4;
  Ciphertext a2 = a + a;
  Ciphertext a3 = a2 + c2;
  Ciphertext inner = b + a3;
  Ciphertext p = w * inner - c4;
  Ciphertext r = p + z * c1;
  r = r + c0;
  r.set_output("y");
}
```