Idea: Clenshaw in the variable w = T2 = 2z² − 1 instead of z, with the even and odd parts handled separately. Since T_{2k}(z) = T_k(w) and T_{2k+3} = 2w·T_{2k+1} − T_{2k−1}, each part is a short recurrence.

- Even part: e2 = c4 + 2w·c6, e1 = c2 + 2w·e2 − c6, and E = c0 + w·e1 − e2.
- Odd part: d2 = c5 + 2w·c7, d1 = c3 + 2w·d2 − c7, d0 = c1 + 2w·d1 − d2, and O = z·(d0 − d1).

Both recurrences are shorter than the degree-7 chain, so the multiplicative depth drops from 8 to about 6 and later operations run at cheaper levels. The ct-ct count stays at 6 and ct-pt multiplies are 3.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext w = u + u - one;
  Ciphertext w2 = w + w;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext e2 = w2 * c6 + c4;
  Ciphertext e1 = w2 * e2 + c2 - c6;
  Ciphertext ev = w * e1 + c0 - e2;
  Ciphertext d2 = w2 * c7 + c5;
  Ciphertext d1 = w2 * d2 + c3 - c7;
  Ciphertext d0 = w2 * d1 + c1 - d2;
  Ciphertext dd = d0 - d1;
  Ciphertext od = z * dd;
  Ciphertext y = od + ev;
  y.set_output("y");
}
```