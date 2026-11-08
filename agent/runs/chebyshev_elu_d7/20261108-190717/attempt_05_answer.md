The circuit stays the same as attempt 2, but the scalar plaintext additions are moved as late as possible. Encoding a plaintext and adding it costs in proportion to the number of primes left at that level, so a constant added deeper in the chain is cheaper.

- **c0:** added once at the very end, at L4 with only 2 primes left, instead of at L2.
- **c4:** added to q after the L2/L3 alignment, at L3, instead of at L2.

The multiplication structure is unchanged: 4 levels, 5 ct-ct and 5 ct-pt multiplications.

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
  Ciphertext zz = z * z;
  Ciphertext t2 = zz + zz - one;       // L2
  Ciphertext t2s = t2 * t2;
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