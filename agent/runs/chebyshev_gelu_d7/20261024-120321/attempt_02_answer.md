The scale mismatch came from `term4` and `term6` both landing at level 4 with different multiplication histories. I now compute T6 = 2·T2·T4 − T2 as a ciphertext first and multiply by c6 last, so `term6` ends at level 5. Every sum then joins terms at different levels: `term1` at level 2, `term2` at 3, `term4` at 4 and `term6` at 5. This uses 4 ciphertext multiplications and a depth of 5, against the baseline's 7 levels.

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
  Ciphertext w = z * z;
  Ciphertext t2 = w + w - one;
  Ciphertext s = t2 * t2;
  Ciphertext t4 = s + s - one;
  Ciphertext q = t2 * t4;
  Ciphertext t6 = q + q - t2;
  Ciphertext term1 = z * c1;
  Ciphertext term2 = t2 * c2;
  Ciphertext term4 = t4 * c4;
  Ciphertext term6 = t6 * c6;
  Ciphertext result = term1 + term2;
  result = result + term4;
  result = result + term6;
  result = result + c0;
  result.set_output("y");
}
```