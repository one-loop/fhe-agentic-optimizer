Idea: use the even/odd structure to lower the multiplication count. With z = alpha·x, set w = z².

The polynomial is c0 + c1 z + c2 T2 + c4 T4 + c6 T6, where
- T2 = 2w − 1
- T4 = 8w² − 8w + 1
- T6 = 32w³ − 48w² + 18w − 1

Collecting terms gives P = c0 + c1 z + A·w + B·w² + C·w³, with
- A = 2c2 − 8c4 + 18c6 (the 2c2 term is c2·2w, i.e. 2c2 − 8c4 + 18c6 on w)
- B = 8c4 − 48c6
- C = 32c6
- constant = c0 − c2 + c4 − c6

Building these coefficients needs plaintext-plaintext arithmetic, which is not allowed. Instead I fold the plaintext coefficients into ciphertext operations.

Cheaper plan: avoid the ciphertext constants by writing the even part as a polynomial in w using plaintext multiplies of ciphertexts.
- Compute T2 = w + w − one. This is a plaintext subtract, and it is allowed because the other operand is a ciphertext.
- T4 = (T2·T2)·2 − 1. Then T6 = 2·T2·T4 − T2.
- The even part is c2·T2 + c4·T4 + c6·T6.

Multiplication count and depth:
- z = x·alpha takes 1 level (a pt multiply).
- w = z² takes 1 level, a ct multiply.
- T2 sits at depth 2. T4 = 2T2² − 1 needs 1 more multiply, so depth 3.
- T6 = 2·T2·T4 − T2 is depth 4.
- The c multiplies add one more level.

An alternative is to reduce the ciphertext multiplications to 3: w, then T2², then T2·T4. That is one fewer than the baseline's 5 ct-ct multiplies, and it skips T3, T5 and T7 entirely.

Further saving: T1 term c1·z = (c1·alpha)·x would need a plaintext-plaintext multiply, so I use (x·alpha)·c1 instead. To save a level, I compute z·c1 from z directly.

Final result: c0 + c1·z + c2·T2 + c4·T4 + c6·T6.

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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * t4;
  Ciphertext t6 = m + m - t2;
  Ciphertext result = z * c1 + t2 * c2 + t4 * c4 + t6 * c6 + c0;
  result.set_output("y");
}
```