Attempt 4 is close to optimal. Fewer multiplications don't seem achievable: depth 4 needs either a 4th ciphertext multiplication (tried in attempt 3) or 4 plaintext multiplications (attempt 1), and both were slower. So this attempt only trims the additions.

Instead of building q3 = 2u − 1 from u, I use v = 2u = 2·T2 directly, with q3 = v − 1 and r = c5·v + c3. The result is the same and the number of multiplications is unchanged. It removes one ciphertext addition, since c5u + c5u becomes a single plaintext multiplication of v.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext z2 = square(z);           // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext v = u + u;                // 2 T2
  Ciphertext q3 = v - one;             // 2u - 1
  Ciphertext r = v * c5 + c3;          // c3 + 2 c5 u, level 3
  Ciphertext inner = q3 * r + c1 - c5; // level 4
  Ciphertext result = z * inner + c0;  // level 5
  result.set_output("y");
}
```