The new best shares w² between the even and odd parts. This attempt keeps that structure and removes plaintext additions by reusing ciphertexts that already contain the same constants. In the odd part, c5 − c7 is already inside `s`, and in the even part c4 is already inside `xe`. That drops 3 plaintext adds, from 9 to 6.

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
  Ciphertext u = square(w);               // w^2

  // even part: (c0-c4) + (c2-3c6) w + u (2c4 + 4c6 w)
  Ciphertext t6 = w * c6;
  Ciphertext t2 = w * c2;
  Ciphertext xe = t6 + t6 + c4;
  Ciphertext xe2 = xe + xe;
  Ciphertext even = (t2 - t6 - xe) + u * xe2 + c0;

  // odd part: z * Q(w)
  Ciphertext t7 = w * c7;
  Ciphertext t5 = w * c5;
  Ciphertext t3 = w * c3;
  Ciphertext s = t7 + t7 + c5 - c7;
  Ciphertext s2 = s + s;
  Ciphertext Y = s2 + s2;
  Ciphertext r = t3 - t5 - t7 - t7;
  Ciphertext lin_o = r + r;
  Ciphertext Q = (lin_o + t7 + t7 - s) + u * Y + c1 - c3;
  Ciphertext odd = z * Q;

  Ciphertext R = odd + even;
  R.set_output("y");
}
```