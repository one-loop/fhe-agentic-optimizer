Attempt 4 was a new circuit but not faster. This attempt shares the ciphertext w² between the even and odd parts, which cuts ct-ct multiplications from 6 to 5. To keep the coefficients as runtime plaintexts, the Chebyshev basis is expanded in w = T2 with the identities:

- T3(z) = z·(2w − 1)
- T5(z) = z·(4w² − 2w − 1)
- T7(z) = z·(8w³ − 4w² − 4w + 1)
- T4(w) = 2w² − 1
- T6(w) = 4w³ − 3w

The cost is 3 more plaintext multiplications, which are cheaper than a ct-ct multiplication at these levels.

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
  Ciphertext lin_e = t2 - t6 - t6 - t6;
  Ciphertext even = lin_e + u * xe2 + c0 - c4;

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