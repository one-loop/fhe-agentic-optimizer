The baseline spends 6 ct-ct multiplies and 8 plaintext multiplies on a degree-7 polynomial, so I'll cut both.

**Idea:** convert to the monomial basis in z and evaluate with a Paterson–Stockmeyer-style scheme. The constants are plaintext inputs, though, so I can't fold the basis conversion into new constants at compile time (rule 2 forbids plaintext-plaintext arithmetic). The conversion therefore has to be expressed through ciphertext operations, which costs extra multiplies.

The alternative is to keep the Chebyshev structure and reduce the multiplication count:
- Use the product identities T_{m+n} = 2 T_m T_n − T_{|m−n|}.
- z = x·alpha costs one plaintext multiply.
- T2 = 2z² − 1 (one square).
- T3 = 2 z T2 − z. T4 = 2 T2² − 1. T6 = 2 T3² − 1.
- T5 = 2 T2 T3 − z. T7 = 2 T3 T4 − T1.
- This needs the squares z², T2², T3² plus the products z·T2, T2·T3, T3·T4. That is 6 ct-ct multiplies, the same as the baseline, but the depth is lower.

Depth is z: 1 (the alpha multiply), T2: 2, T3: 3, T4: 3, T5: 4, T6: 4, T7: 4. After the coefficient multiplies the depth is 5, against 8 in the baseline. Shallower levels make every later operation cheaper.

To avoid the 2× terms costing a scalar multiply, I write them as additions. The scale-mismatch rule applies where terms meet, but CHEHAB aligns operands at different levels. The terms c_k·T_k are all at different levels or the same pattern, so they should be fine.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;
  Ciphertext p3 = z * t2;
  Ciphertext t3 = p3 + p3 - z;
  Ciphertext q4 = square(t2);
  Ciphertext t4 = q4 + q4 - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext q6 = square(t3);
  Ciphertext t6 = q6 + q6 - one;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + t3 * c3;
  result = result + t4 * c4;
  result = result + t5 * c5;
  result = result + t6 * c6;
  result = result + t7 * c7;
  result = result + c0;
  result.set_output("y");
}
```