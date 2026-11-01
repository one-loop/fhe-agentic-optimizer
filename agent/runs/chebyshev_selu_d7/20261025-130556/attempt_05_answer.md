Idea: rewrite the whole polynomial as a Horner scheme in T2, using T4 = 2·T2² − 1 and T6 = T2·(2·T4 − 1). This needs only 4 ct-ct multiplications (z², T2·R, T2·(2g+4S), and the final T2 product) and no T4 at all. The depth is 5, one more than the current best, but one ciphertext multiplication is saved.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext T2 = z2 + z2 - one;

  Ciphertext zc1 = z * c1;
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext A = ((zc3 + zc3) + c2) - ((zc5 + zc5) + c6);
  Ciphertext R = (zc7 + zc7) + c6;
  Ciphertext S = T2 * R;
  Ciphertext w = zc5 - zc7;
  Ciphertext g2 = (w + w) + c4;
  Ciphertext g = g2 + g2;
  Ciphertext S4 = (S + S) + (S + S);
  Ciphertext P = T2 * (g + S4);
  Ciphertext U = A - (R + R);
  Ciphertext F = T2 * (U + P);
  Ciphertext lin = (zc1 + zc5) - (zc3 + zc7);
  Ciphertext rem = ((lin - g2) + c0);
  Ciphertext r = rem + F;
  r.set_output("y");
}
```

Wait: g in the derivation is c4+2w, which is g2 here. So the Horner bracket needs 2g = g2+g2, which is what `g` holds, and the constant term subtracts g2. That matches the code above.