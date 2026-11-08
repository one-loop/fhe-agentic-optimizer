The current best is attempt 1 at 90.21 ms, with 4 levels and 5 ciphertext multiplications. For this attempt I kept its math and changed only two operations, so it is a small experiment rather than a new idea. `z*z` and `T2*T2` are now written as `square()`. That changes the circuit's operation counts (2 squares and 3 ciphertext multiplies instead of 5 multiplies). SEAL's dedicated squaring is usually somewhat cheaper than a general multiply, but whether it wins here is untested.

I checked several ways to save more and found none:
- **4 multiplies:** this needs 5 levels, or a T3-based form that costs about 10 plaintext multiplies.
- **3 levels:** dropping the `alpha` level would need constants like 1/32 or 1/64, which only exist as extra `alpha` multiplies costing levels.
- **No scale alignments:** removing the 3 alignments would need about 8 extra plaintext adds, which would likely cost more.

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

  Ciphertext z = x * alpha;                 // L1
  Ciphertext zz = square(z);                // L2
  Ciphertext T2 = zz + zz - one;            // L2
  Ciphertext T4s = square(T2);              // L3
  Ciphertext T4 = T4s + T4s - one;          // L3

  Ciphertext zc1 = z * c1;                  // L2
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  // high part q = (c5-c7) z + T2*(c6 + 2 c7 z)
  Ciphertext r = zc7 + zc7 + c6;            // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = (zc5 - zc7) + h;           // L3
  Ciphertext P = T4 * (q + q + c4);         // L4

  // low part: (c1-c7-c3+c5) z + T2*((c2-c6) + 2(c3-c5) z)
  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = s + s + c2 - c6;           // L2
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = zc1 - zc7 - zc3 + zc5;   // L2

  Ciphertext y = lin + g + P + c0;
  y.set_output("y");
}
```