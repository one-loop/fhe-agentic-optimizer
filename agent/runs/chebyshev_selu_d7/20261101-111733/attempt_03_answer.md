Attempt 2 was only about 1% faster, so this kernel goes for depth 4. It no longer builds T3 and multiplies it by plaintexts, which is what pushed the T3 terms to level 4 and the final product to level 5.

The T3 terms are now built as products of two level-2 ciphertexts, using u = 2T2 − 1 and T3 = T1·u:
- c7·T3 = (c7·T1)·u
- (c3 − c5)·T3 = (c3·T1 − c5·T1)·u

The products c7·T1 and c5·T1 are shared with the low part and with the inner sum.

Expected effect:
- Plaintext multiplications drop from 9 to 7.
- Ciphertext multiplications rise from 4 to 5, but they run at shallower levels.
- The circuit consumes 4 levels instead of 5, which makes every later operation cheaper.

Risk: some level-3 additions combine terms with different multiplication histories. If CHEHAB reports a scale mismatch, I will restructure those sums.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext T1 = x * alpha;
  Ciphertext zz = T1 * T1;
  Ciphertext T2 = zz + zz - one;
  Ciphertext u = T2 + T2 - one;
  Ciphertext t22 = T2 * T2;
  Ciphertext T4 = t22 + t22 - one;

  Ciphertext n = T1 * c7;
  Ciphertext a5 = T1 * c5;
  Ciphertext m = T1 * c3 - a5;
  Ciphertext l1 = T1 * c1 - n;

  Ciphertext a6 = T2 * c6;
  Ciphertext l2 = T2 * c2 - a6;

  Ciphertext nu = n * u;
  Ciphertext mu = m * u;

  Ciphertext q = nu + a6 + a5;
  Ciphertext inner = q + q + c4;
  Ciphertext high = T4 * inner;

  Ciphertext result = high + mu + l2 + l1 + c0;
  result.set_output("y");
}
```