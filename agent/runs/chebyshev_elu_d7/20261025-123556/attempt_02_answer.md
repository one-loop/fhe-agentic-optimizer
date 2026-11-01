The attempt-1 error came from two terms meeting at level 4 with different multiplication histories: t3·c3 and t4·c4. I changed t4 so its term lands one level deeper.

- t4 for the output sum is now built as 2·z·t3 − t2, which is depth 4, as in the baseline. Its term then sits at level 5, apart from t3·c3 at level 4.
- t7 still uses the shallower t4 = 2·t2² − 1 (depth 3) so it stays at depth 4. This costs one extra ciphertext multiplication.
- I have not verified that the remaining level-5 terms (t4, t5, t6, t7) share the same scale pattern, so this attempt may still hit a mismatch.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext w3 = z * t2;
  Ciphertext t3 = w3 + w3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4a = s4 + s4 - one;
  Ciphertext w4 = z * t3;
  Ciphertext t4b = w4 + w4 - t2;
  Ciphertext w5 = t2 * t3;
  Ciphertext t5 = w5 + w5 - z;
  Ciphertext s6 = square(t3);
  Ciphertext t6 = s6 + s6 - one;
  Ciphertext w7 = t3 * t4a;
  Ciphertext t7 = w7 + w7 - z;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4b * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```