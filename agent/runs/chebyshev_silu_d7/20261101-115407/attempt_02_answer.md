This attempt evaluates the even part as a Horner polynomial in u = T2. That brings the plaintext multiplications down to 3 (`alpha`, `c6`, `c1`), one fewer than the best. The cost is one more multiplicative level, depth 5 instead of 4.

The expansion in u = T2 = 2z² − 1 is:
- c2·T2 + c4·T4 + c6·T6 = 4c6·u³ + 2c4·u² + (c2 − 3c6)·u − c4
- That equals u·(u·(4c6·u + 2c4) + c2) − 3c6·u − c4.

All coefficients beyond c6 come in through plaintext additions or from reusing s = c6·u. The circuit still has 3 ciphertext multiplications.

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
  Ciphertext u = w + w - one;          // T2
  Ciphertext s = u * c6;               // c6 u
  Ciphertext h = s + s + c4;           // 2 c6 u + c4
  Ciphertext inner = h + h;            // 4 c6 u + 2 c4
  Ciphertext mid = u * inner + c2;     // 4 c6 u^2 + 2 c4 u + c2
  Ciphertext e = u * mid;              // 4 c6 u^3 + 2 c4 u^2 + c2 u
  Ciphertext lin = z * c1;
  Ciphertext result = e - (s + s + s) + lin + c0 - c4;
  result.set_output("y");
}
```