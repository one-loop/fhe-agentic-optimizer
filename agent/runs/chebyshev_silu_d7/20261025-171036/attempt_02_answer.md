Idea: cut ct-ct multiplications and depth further by using the structure of the polynomial.

Current: w = x·alpha (pt mul), u = w² (mul), t2, t4 = 2t2²−1 (mul), t6 (mul), then 4 plaintext multiplies.

Improvements:
- Skip the w multiplication on the even path: u = z² = x²·alpha². Computing x² first (square, level 1) and multiplying by alpha² would need alpha·alpha, which is plaintext-plaintext arithmetic and not allowed. Instead, (x·alpha) is kept for w, but I can square x directly and use the odd term c1·alpha·x = (x·c1)·alpha, which is still two plaintext multiplies. So that doesn't help.
- Reduce the plaintext multiplies: the sum c2·t2 + c4·t4 + c6·t6 + c0 can be evaluated in a different order. With t4 = 2t2²−1 and t6 = 2·t2·t4 − t2 = t2·(2t4 − 1), I get t6·c6 = t2·((t4+t4−one)·c6). That does not reduce the count.
- Fold c6 earlier: t6·c6 = (t2·c6)·(t4+t4−one) = t2c6·(2t4−1). t2c6 is computed at level 2 in parallel with t4 (also at level 3), so the multiply sits at level 4 as before, but the pt multiply moves off the critical path, which saves a rescale-level alignment. The same trick works for c4: c4·t4 = c4·(2s−1) = (s·c4)+(s·c4) − c4, and the plaintext c4 is added at a ciphertext level, so no extra cost.

Let me define:
- a2 = t2·c2 (level 3)
- s = t2², sc4 = s·c4, so c4·t4 = sc4 + sc4 − c4
- m = t2·c6, t6·c6 = m·(2t4−1) = 2·m·t4 − m

That keeps the same ct-ct count (3) but gives the same pt-mul count with a shallower tail, since m·t4 lands at level 4 either way. The gain is small but the circuit differs.

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
  Ciphertext t4 = s + s - one;
  Ciphertext sc4 = s * c4;
  Ciphertext m = t2 * c6;
  Ciphertext q = m * t4;
  Ciphertext result = w * c1;
  result = result + t2 * c2;
  result = result + sc4;
  result = result + sc4;
  result = result - c4;
  result = result + q;
  result = result + q;
  result = result - m;
  result = result + c0;
  result.set_output("y");
}
```