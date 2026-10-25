This attempt splits the polynomial into even and odd parts in t = T2(z) = 2z² − 1, since T_{2j}(z) = T_j(t). Both parts are evaluated with Clenshaw recurrences in t, and the odd part reduces to z·(d0 − d1).

- **Multiplications:** still 6 ciphertext multiplications, with only 3 plaintext multiplications.
- **Depth:** the longest path drops to about 6 instead of 8, which makes the later operations cheaper.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t = z2 + z2 - one;
  Ciphertext w = t + t;

  // even part: sum c_{2j} T_j(t)
  Ciphertext pe = w * c6;
  Ciphertext e2 = pe + c4;
  Ciphertext e1 = w * e2 + c2 - c6;
  Ciphertext ev = t * e1 + c0 - e2;

  // odd part: z * (d0 - d1)
  Ciphertext po = w * c7;
  Ciphertext d2 = po + c5;
  Ciphertext d1 = w * d2 + c3 - c7;
  Ciphertext d0 = w * d1 + c1 - d2;
  Ciphertext diff = d0 - d1;
  Ciphertext od = z * diff;

  Ciphertext y = ev + od;
  y.set_output("y");
}
```