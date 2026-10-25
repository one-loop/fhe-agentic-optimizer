Attempt 4 is the best so far at 334.5 ms. I can remove one plaintext multiplication: c6·T2 appears twice (inside the inner product and in the linear part), so I compute it once and reuse it. The same product is subtracted in the linear part and doubled in the inner product. That reduces the plaintext multiplications from 9 to 8, with no change to depth or the ciphertext multiplication count.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");

  // z = x/4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext z2 = z * z;
  Ciphertext t2 = z2 + z2 - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;

  // T4 = 2 T2^2 - 1  (depth 3)
  Ciphertext t2sq = t2 * t2;
  Ciphertext t4 = t2sq + t2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  // shared plaintext products
  Ciphertext u5a = z * c5;     // c5 T1       (level 2)
  Ciphertext u7b = z * c7;     // c7 T1       (level 2)
  Ciphertext u6 = t2 * c6;     // c6 T2       (level 3)
  Ciphertext u5b = t3 * c5;    // c5 T3       (level 4)
  Ciphertext u7a = t3 * c7;    // c7 T3       (level 4)
  Ciphertext p1 = z * c1;      // c1 T1       (level 2)
  Ciphertext p2 = t2 * c2;     // c2 T2       (level 3)
  Ciphertext p3 = t3 * c3;     // c3 T3       (level 4)

  // inner B = 2c5 T1 + 2c6 T2 + 2c7 T3 + c4   (depth 4)
  Ciphertext d5 = u5a + u5a;
  Ciphertext d6 = u6 + u6;
  Ciphertext d7 = u7a + u7a;
  Ciphertext B = d5 + d6;      // levels 2,3 -> 3
  B = B + d7;                  // levels 3,4 -> 4
  B = B + c4;                  // plaintext add

  // product T4 * B  (depth 5)
  Ciphertext prod = t4 * B;

  // linear part: (c1-c7) T1 + (c2-c6) T2 + (c3-c5) T3 + c0
  Ciphertext D1 = p1 - u7b;    // level 2
  Ciphertext D2 = p2 - u6;     // level 3
  Ciphertext D3 = p3 - u5b;    // level 4
  Ciphertext O = D1 + D2;      // levels 2,3 -> 3
  O = O + D3;                  // levels 3,4 -> 4
  O = O + c0;                  // plaintext add

  Ciphertext result = prod + O;   // levels 5,4 -> 5
  result.set_output("y");
}
```