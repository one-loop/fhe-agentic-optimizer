The T3/T4 and T5/T6/T7 terms kept landing at the same level with different scales. This attempt avoids that by splitting the polynomial into its even and odd parts.

- **Even part:** the even terms, T0, T2, T4 and T6, are evaluated with Clenshaw's recurrence in `w = T2`. This needs no T4 or T6 ciphertext and has 2 ct-ct multiplications.
- **Odd part:** T3, T5 and T7 use `T7 = 2·T2·T5 − T3`, so the odd terms sit at distinct levels (2, 4, 5, 6).
- **Summation order:** the sum is arranged so that terms meeting at the same level share the same multiplication history, or the sum is aligned from a different level:
  - The even part (level 5) is added to the c1 term (level 2).
  - The T3 and T7 terms are combined next, and the result is added to that sum.
  - The T5 term (level 5) is added last, against a level-6 sum.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext w = z2 + z2 - one;           // T2
  Ciphertext a = z * w;
  Ciphertext T3 = a + a - z;
  Ciphertext c = w * T3;
  Ciphertext T5 = c + c - z;
  Ciphertext f = w * T5;
  Ciphertext T7 = f + f - T3;

  // even part by Clenshaw in w
  Ciphertext t = w * c6;
  Ciphertext b2 = t + t + c4;
  Ciphertext p = w * b2;
  Ciphertext b1 = p + p + c2 - c6;
  Ciphertext even = w * b1 - b2 + c0;

  Ciphertext E = even + z * c1;
  Ciphertext O = T3 * c3 + T7 * c7;
  Ciphertext R = O + E;
  R = R + T5 * c5;
  R.set_output("y");
}
```