Idea: combine the two small wins. Use the shared-product structure from attempt 5 (8 plaintext multiplications, B built from `e = z·T2`) together with the T4-free final step 2·(T2²·B) − B from attempt 6. This removes the T4 plaintext add and the duplicate plaintext products.

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
  Ciphertext e = z * t2;
  Ciphertext t3 = e + e - z;
  Ciphertext t2s = square(t2);

  Ciphertext zc1 = z * c1;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;
  Ciphertext t2c2 = t2 * c2;
  Ciphertext t2c6 = t2 * c6;
  Ciphertext ec7 = e * c7;

  // B = c4 + 2(c5-c7) z + 2 c6 T2 + 4 c7 e
  Ciphertext bz = zc5 - zc7;
  Ciphertext b = bz + bz;
  b = b + (t2c6 + t2c6);
  Ciphertext e2 = ec7 + ec7;
  b = b + (e2 + e2);
  b = b + c4;
  Ciphertext m = t2s * b;
  Ciphertext hi = m + m - b;

  // A = c0 + (c1-c7) z + (c2-c6) T2 + (c3-c5) T3
  Ciphertext a = zc1 - zc7;
  a = a + (t2c2 - t2c6);
  Ciphertext t3c3 = t3 * c3;
  Ciphertext t3c5 = t3 * c5;
  a = a + (t3c3 - t3c5);
  a = a + c0;

  Ciphertext r = a + hi;
  r.set_output("y");
}
```