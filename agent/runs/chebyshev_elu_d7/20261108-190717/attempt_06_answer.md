I could not find a structurally cheaper circuit. A degree-7 polynomial whose coefficients are linear ciphertexts in z seems to need 5 ciphertext multiplications at the minimum 4 levels. The 4 scalar multiplications on z cover the 4 independent combinations of c1, c3, c5 and c7, so they cannot be reduced either.

This attempt therefore combines two small gains that each lost or tied separately:
- **Squaring:** z·z and T2·T2 go through SEAL's `square`, which needs fewer polynomial products than a general multiply.
- **Late constants:** c0 is added at L4 and c4 at L3, where each plaintext encoding carries fewer primes.

The multiplication structure stays the same as attempt 2: 4 levels, 5 ct-ct and 5 ct-pt multiplications.

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

  Ciphertext z = x * alpha;            // L1
  Ciphertext zz = square(z);
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;     // L3

  Ciphertext a1 = z * c1;              // L2
  Ciphertext a3 = z * c3;
  Ciphertext a5 = z * c5;
  Ciphertext a7 = z * c7;

  // high part q = c4 + 2(c5-c7) z + 2 T2 (c6 + 2 c7 z)
  Ciphertext v = a7 + a7 + c6;         // L2
  Ciphertext hv = v * t2;              // L3
  Ciphertext d57 = a5 - a7;            // L2
  Ciphertext q = (hv + hv + (d57 + d57)) + c4;   // c4 added at L3

  // low part (without c0): (c1-c7) z - g + T2 (2g + c2 - c6), g = (c3-c5) z
  Ciphertext g = a3 - a5;              // L2
  Ciphertext m = g + g + c2 - c6;      // L2
  Ciphertext lm = m * t2;              // L3
  Ciphertext low = lm + (a1 - a7 - g);

  Ciphertext y = (t4 * q + low) + c0;  // c0 added at L4
  y.set_output("y");
}
```