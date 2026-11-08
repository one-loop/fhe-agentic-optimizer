Attempt 4 reached about 2% over the best (87.3 ms) but not the required 3%. Its 3 scale alignments are what remain to attack.

This attempt keeps attempt 4's math and changes only where the L2 linear term `lin` enters the sum. Before, it was added to the L3 terms and then the result was aligned again to L4. Now the L3 terms meet the L4 product first, and `lin` and `c0` are added last at L4. The alignment of `lin` then happens where its ciphertext has the fewest primes. CHEHAB may also use a mod switch there instead of a multiply-plain plus rescale. I have not confirmed CHEHAB keeps this order, so this attempt is a test.

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
  Ciphertext T4s = square(T2);              // L3  (T4 = 2*T4s - 1)

  Ciphertext zc1 = z * c1;                  // L2
  Ciphertext zc3 = z * c3;
  Ciphertext zc5 = z * c5;
  Ciphertext zc7 = z * c7;

  Ciphertext r = zc7 + zc7 + c6;            // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = (zc5 - zc7) + h;           // L3
  Ciphertext w = q + q + c4;                // L3
  Ciphertext P = T4s * (w + w);             // L4

  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = s + s + c2 - c6;           // L2
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = zc1 - zc7 - s;           // L2

  Ciphertext hi = (g - w) + P;              // L4
  Ciphertext y = (hi + lin) + c0;
  y.set_output("y");
}
```