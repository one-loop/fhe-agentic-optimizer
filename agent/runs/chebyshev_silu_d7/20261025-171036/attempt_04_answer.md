Idea: attempt 3 was a bit faster but under the 3% margin. Reduce depth and operations further.

The even part is a cubic in u = z²: E(u) = c0 + c2·t2 + c4·t4 + c6·t6. Currently depth is 5 because t2 comes after w·w (levels 1 and 2), then s (3), then the A·c6 product (4) and the final multiply by t2 (5).

Option: compute u = x² first, from x directly (square, level 1), so z² = alpha²·x². The plaintext alpha² cannot be built, but u = (x·alpha)·(x·alpha) needs w anyway. Instead use p = x·x (level 1) and q = alpha·x (level 1, in parallel), then u = p·alpha·alpha costs two plaintext multiplies, which is more ops but on a shorter path. That is a different trade-off and not clearly better.

Better: reduce the number of ops on the t2 chain. Write t2 = 2u−1 and s = t2². Then A = 4s−3 and the term is t2·(c2 + c6·A) + c4·(2s−1).
Alternative: put c6 in earlier: m = t2·c6 (level 3, parallel with s), then D = m·A + c2·t2. This moves the pt multiply off the critical path: A at level 3, m at level 3, m·A at level 4. Depth drops to 4. The scale-mismatch risk: m·A (history: pt, ct×ct chain) lands at level 4, while the other terms are s·c4 (level 4) and c2·t2 (level 3). To avoid same-level mismatches I put c4 on s one level up: E = s·c4 sits at level 4 with a different history from m·A, which caused the attempt 2 error. Fix: let c4·s be merged before the multiply as part of the polynomial: m·A + c4·(2s−1) = m·A + 2·(s·c4) − c4. Instead make it share the pattern by multiplying: (s·c4) is pt-mul on a level-3 value, m·A is pt-mul then ct-mul. Different. So I instead fold c4 into A via a factor: m2 = t2·c6 (level 3) and F = A·m2... still the same issue.

Safer: keep m·A at level 4 and force the others to be shallower by combining them first into a value multiplied once more. I take G = (c2·t2 + c4·(2s−1)·…) which stays at level 4 anyway. So I accept the risk-free structure of attempt 3 and instead just cut the w·c1 term: c1·w = (x·c1)·alpha costs the same. I'll try the m-based version with c4 handled via t4 = 2s−1 at level 3 multiplied by c4 at level 4, and m·A at level 4 — the same-level mismatch is likely. To dodge it, I multiply c4 term with t2-level history: c4·t4 + c6·t6 = t4·c4 + m·A, forcing both to be ct×ct products by turning c4·t4 into (t2·c4)·t2-style: (t2·c4)·t2 = c4·s. That has the same history as m·A (pt-mul, then ct-mul with a level-3 operand), so scales match.

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
  Ciphertext m = t2 * c6;
  Ciphertext n = t2 * c4;
  Ciphertext P = m * A;
  Ciphertext Q = n * t2;
  Ciphertext result = P + Q;
  result = result + Q;
  result = result + t2 * c2;
  result = result - c4;
  result = result + w * c1;
  result = result + c0;
  result.set_output("y");
}
```