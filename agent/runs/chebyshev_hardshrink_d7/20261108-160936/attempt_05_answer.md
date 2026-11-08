Since T5 and T7 both contain T3, I factor it out: c3·T3 + c5·T5 + c7·T7 = T3·(c3 + 2·T2·c5 + 2·T4·c7) − z·(c5 + c7). The `inner` sum mixes T2·c5 (level 3) and T4·c7 (level 4), which CHEHAB aligns for free. The three z terms are combined at level 2. This needs 4 ciphertext-ciphertext multiplications (down from 5) and 6 plaintext multiplications (down from 7), plus one plaintext add for c3. I haven't compiled it, so scale alignment is still unchecked.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext m = t2 * c5 + t4 * c7;
  Ciphertext inner = m + m + c3;
  Ciphertext w = z * c1 - z * c5 - z * c7;
  Ciphertext result = t3 * inner + w;
  result.set_output("y");
}
```