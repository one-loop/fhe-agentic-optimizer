Idea: the previous failure came from two terms with different multiplication histories landing on the same level. I'll go back to the best kernel and cut work in a safer way.

Key change: the odd term w·c1 needs w = x·alpha first. Using x directly, w·c1 becomes x·(alpha·c1), which is plaintext-plaintext arithmetic and not allowed. So I keep w, but reuse it: u = w² is unchanged.

The reduction I make is to fold c6 and c4 into the polynomial in t2 to cut plaintext multiplies:
- The even part is c0 + c2·t2 + c4·(2s−1) + c6·t2·(2t4−1), where s = t2².
- Rewrite it as a quadratic in s and t2: t6 = t2·(4s − 3), because t2·(2(2s−1) − 1) = t2·(4s − 3).
- Then c6·t6 = (c6·t2)·(4s − 3). With 4s−3 = (s+s+s+s) − (one+one+one), I add one as plaintext three times, which is allowed since a ciphertext is the other operand.
- Group the terms: t2·(c2 + c6·(4s−3)) + c4·(2s−1). The c4 term gives 2·(s·c4) − c4.

To avoid the scale mismatch, I build every term as a product that has passed through the same pattern of multiplications:
- A = (s+s+s+s) − 3·one (level 3, multiplication history: 3 ct-ct mul)
- B = A·c6 (level 4)
- C = B + c2 (c2 is a plaintext add, so no scale issue)
- D = C·t2 (level 5)

This needs only 3 ct-ct multiplications (w², s, final t2 product) and the plaintext multiplies are w·c1, A·c6, s·c4. Total 3 ct-pt multiplications instead of 5 (the one for x·alpha stays). The scale-mismatch risk is in the final sum. D is at level 5 and the others are shallower, so they meet at different levels and are aligned for free.

Depth: w (1), u (2), t2 (2), s (3), A (3), B (4), D (5). Same depth as before but fewer operations.

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
  Ciphertext w = x * alpha;
  Ciphertext u = w * w;
  Ciphertext t2 = u + u - one;
  Ciphertext s = t2 * t2;
  Ciphertext s2 = s + s;
  Ciphertext A = s2 + s2 - one - one - one;
  Ciphertext B = A * c6;
  Ciphertext C = B + c2;
  Ciphertext D = C * t2;
  Ciphertext E = s * c4;
  Ciphertext result = D + w * c1;
  result = result + E + E;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```