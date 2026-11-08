Idea: keep the plaintext-add trick from Clenshaw, but switch to a split around T4 to cut ct-ct multiplies from 6 to 4. Using T5 = 2T4·T1 − T3, T6 = 2T4·T2 − T2 and T7 = 2T4·T3 − T1, I get y = [c0 + (c1−c7)T1 + (c2−c6)T2 + (c3−c5)T3] + T4·(c4 + 2(c5T1 + c6T2 + c7T3)). The plaintext differences are expressed as subtracted ct×pt products, since plaintext-plaintext arithmetic isn't allowed. Terms that meet in a sum share a level and multiplication history: the T1, T2 and T3 products each sit at their own level, and the T4 product sits at yet another.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = t2 * t2;
  Ciphertext t4 = t2s + t2s - one;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext q = z * c5 + t2 * c6 + t3 * c7;
  Ciphertext w = q + q + c4;
  Ciphertext hi = t4 * w;
  Ciphertext l1 = z * c1 - z * c7;
  Ciphertext l2 = t2 * c2 - t2 * c6;
  Ciphertext l3 = t3 * c3 - t3 * c5;
  Ciphertext y = hi + l3 + l2 + l1 + c0;
  y.set_output("y");
}
```