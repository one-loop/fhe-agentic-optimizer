Depth 5 is already close to minimal for a degree-7 polynomial plus the alpha multiplication. This attempt therefore targets the cheaper overheads, mainly scale alignments and the add/encode pattern.

- **Doubling moved before the coefficients:** `(T_j+T_j)*c` replaces the doubled sum `q+q`. The constant c4 is then added to a sum that is already at the deepest level.
- **Low part regrouped:** the three coefficient pairs are grouped by T-index, (c1−c7)T1, (c2−c6)T2 and (c3−c5)T3. They are added from the deepest level down, with c0 added last.

These changes should alter the alignment pattern CHEHAB inserts. I can't predict whether they reduce `match_scale`.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;
  Ciphertext zt2 = t1 * t2;
  Ciphertext t3 = zt2 + zt2 - t1;
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;

  Ciphertext d1 = t1 + t1;
  Ciphertext d2 = t2 + t2;
  Ciphertext d3 = t3 + t3;

  // s = c4 + 2 c5 T1 + 2 c6 T2 + 2 c7 T3
  Ciphertext s = d3 * c7 + d2 * c6 + d1 * c5 + c4;
  Ciphertext top = t4 * s;

  // low = (c3 - c5) T3 + (c2 - c6) T2 + (c1 - c7) T1 + c0
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext l2 = t2 * c2 - t2 * c6;
  Ciphertext l1 = t1 * c1 - t1 * c7;
  Ciphertext low = l3 + l2 + l1;

  Ciphertext y = top + low + c0;
  y.set_output("y");
}
```