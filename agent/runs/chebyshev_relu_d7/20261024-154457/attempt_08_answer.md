Last attempt: I regroup the additions so that terms with the same multiplication history meet first. The two level-3 terms (c2·T2 and the c6·T2 correction) are combined, then added to the level-4 product, and the shallow c1·z term joins last. This may reduce the scale alignments CHEHAB inserts, while the multiplication counts stay at 3 ciphertext and 4 plaintext.

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
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext p = t4 * m;
  Ciphertext mid = t2 * c2 - a6;
  Ciphertext s = p + mid;
  Ciphertext result = s + z * c1;
  result = result + c0;
  result.set_output("y");
}
```