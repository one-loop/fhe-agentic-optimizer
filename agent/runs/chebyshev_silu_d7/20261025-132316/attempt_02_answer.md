Idea: reorder the T6 term so the c6 multiplication happens early. Since T6 = v(2·T4 − 1) with v = T2, I compute m = c6·v right after v is available, in parallel with T4. Then c6·T6 = m·(T4 + T4 − one), which needs one ct-ct multiplication. This cuts the multiplicative depth from 5 to 4, and the remaining operations run at deeper, cheaper levels.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * c6;
  Ciphertext w = t4 + t4 - one;
  Ciphertext r6 = m * w;
  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t4 * c4;
  r = r + r6;
  r = r + c0;
  r.set_output("y");
}
```