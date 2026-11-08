Idea: the mismatch came from adding the c5·T5 and c7·T7 terms at the same level with different multiplication histories. I now distribute the coefficients into the products so that terms with different histories meet at different levels:
- c7·T7 becomes 2·(c7·T3)·T4 − c7·z, which ends at level 5.
- c5·T5 becomes 2·(c5·z)·T4 − c5·T3, which ends at level 4.
- The two big product terms are summed first, so they meet at different levels.
- The lower-level terms are then added one at a time into that running sum.

I have not compiled this, so the scales may still need another pass.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext w = (z * c5) * t4;
  Ciphertext v = (t3 * c7) * t4;
  Ciphertext result = v + v;
  result = result + (w + w);
  result = result + t3 * c3;
  result = result - t3 * c5;
  result = result + z * c1;
  result = result - z * c7;
  result = result + c0;
  result.set_output("y");
}
```