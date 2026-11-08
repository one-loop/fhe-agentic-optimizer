**Fix:** in attempt 1, c3·T3 and c4·T4 met at level 4 with different scales. T3 comes from z·T2 and T4 from T2·T2, so their multiplication histories differ.

I now fold c4 into the giant-step factor instead of multiplying T4 by c4 separately:
c4·T4 + 2·T4·A = T4·(2A + c4).

This removes the mismatched addition and saves one plaintext multiplication. The circuit now has 4 ct-ct multiplications, 8 ct-pt multiplications and depth 5.

Within A, within B and within low, the terms sit at different levels (2, 3 and 4). B and low meet at level 4 with the same history (a plaintext times T3), and w sits at level 5.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext t1 = x * alpha;                 // level 1
  Ciphertext zz = t1 * t1;
  Ciphertext t2 = zz + zz - one;             // level 2
  Ciphertext zt2 = t1 * t2;
  Ciphertext t3 = zt2 + zt2 - t1;            // level 3
  Ciphertext t22 = t2 * t2;
  Ciphertext t4 = t22 + t22 - one;           // level 3

  Ciphertext c6t2 = t2 * c6;
  Ciphertext a = t1 * c5 + c6t2 + t3 * c7;   // level 4
  Ciphertext b = t3 * c5 + c6t2 + t1 * c7;   // level 4
  Ciphertext low = t1 * c1 + t2 * c2 + t3 * c3; // level 4

  Ciphertext a2 = a + a + c4;
  Ciphertext w = t4 * a2;                    // level 5
  Ciphertext s = low - b;
  Ciphertext result = w + s + c0;
  result.set_output("y");
}
```