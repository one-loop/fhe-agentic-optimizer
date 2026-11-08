Attempt 6's slower timing is probably noise, since its operation counts are lower or equal. As a variant, I keep the odd part of attempt 5 and apply the constant-reuse trick only to the even part.

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

  // even part
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
  Ciphertext Q = lin_o + u * Y + c1 - c3 - c5 + c7;
  Ciphertext odd = z * Q;

  Ciphertext R = odd + even;
  R.set_output("y");
}
```