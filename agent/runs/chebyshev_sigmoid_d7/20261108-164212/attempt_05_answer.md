The previous kernel needs T4 only for c7·T4. Since T4 = 2·T2² − 1, the inner factor becomes T2·(4·c7·T2 + 2·c5) + c3 − 2·c7. This removes the separate T2² multiplication and one plaintext multiplication, and the constants are added as plaintexts. It still has 4 ciphertext-ciphertext multiplications but only 5 plaintext multiplications, at the same depth of 5. I have not compiled or timed it.

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
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext m = t2 * c7;
  m = m + m;
  m = m + m;
  m = m + c5;
  m = m + c5;
  Ciphertext inner = t2 * m;
  inner = inner + c3;
  inner = inner - c7;
  inner = inner - c7;
  Ciphertext big = t3 * inner;
  Ciphertext zt = z * c1 - z * c5 - z * c7;
  Ciphertext result = big + zt;
  result = result + c0;
  result.set_output("y");
}
```