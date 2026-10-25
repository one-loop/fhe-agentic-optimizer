The level-5 mismatches suggest that same-level terms with different histories are the problem in the deep products. This version uses the shallow identity only for `T4 = 2·T2² − 1` and keeps the baseline-style chain `z*t_{k-1}` for `T5`, `T6` and `T7`. That puts the terms `c5*t5`, `c6*t6`, `c7*t7` at levels 5, 6 and 7, all distinct. The only same-level pair is `c3*t3` and `c4*t4`, which share a history and are summed first. Everything is then added in ascending level order.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;
  Ciphertext zt4 = z * t4;
  Ciphertext t5 = zt4 + zt4 - t3;
  Ciphertext zt5 = z * t5;
  Ciphertext t6 = zt5 + zt5 - t4;
  Ciphertext zt6 = z * t6;
  Ciphertext t7 = zt6 + zt6 - t5;

  Ciphertext r = z * c1;
  r = r + t2 * c2;
  Ciphertext g = t3 * c3 + t4 * c4;
  r = r + g;
  r = r + t5 * c5;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```