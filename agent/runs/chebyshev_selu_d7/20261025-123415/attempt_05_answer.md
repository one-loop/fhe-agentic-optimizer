Attempt 3 was the faster of the last two, so I build on it and drop the plaintext subtraction `d = 2*t2 - one`. `t3` is now computed as `4*z*z2 - 3*z` using only ciphertext adds, which should save a plaintext encoding and a scale alignment.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext z3 = z * z2;
  Ciphertext a = z3 + z3;
  a = a + a;
  Ciphertext zz = z + z + z;
  Ciphertext t3 = a - zz;
  Ciphertext sq = t2 * t2;
  Ciphertext t4 = sq + sq - one;
  Ciphertext u = t2 * c6;
  Ciphertext q = z * c5 + u + t3 * c7;
  Ciphertext q2 = q + q + c4;
  Ciphertext r = z * c7 + u + t3 * c5;
  Ciphertext m = t4 * q2;
  Ciphertext low = z * c1 + t2 * c2 + t3 * c3;
  Ciphertext y = low - r + m;
  y = y + c0;
  y.set_output("y");
}
```