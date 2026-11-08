The current best is attempt 1 at 90.21 ms. I don't expect a big win here; this variant saves roughly one plaintext encoding.

Earlier, T4 = 2·T2² − 1 needed a plaintext subtraction of `one`. Instead I expand P = T4·w = 2·T2²·w − w, with w = 2q + c4. The `−w` term already carries c4 inside the ciphertext, so the separate `− one` plaintext add for T4 goes away. The circuit is still 4 levels and 5 ciphertext multiplications, using `square()`.

I don't think either the multiplications or the levels can come down further under these rules:
- **Levels:** the x⁷ term needs a coefficient times α⁴·x⁷, which already needs depth 4.
- **Multiplications:** a 4-multiply split would need products of coefficients, which aren't allowed without plaintext–plaintext arithmetic.

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

  // q = (c5-c7) z + T2*(c6 + 2 c7 z);  w = 2q + c4
  Ciphertext r = zc7 + zc7 + c6;            // L2
  Ciphertext h = T2 * r;                    // L3
  Ciphertext q = (zc5 - zc7) + h;           // L3
  Ciphertext w = q + q + c4;                // L3
  Ciphertext P = T4s * (w + w);             // L4  (T4*w = 2 T4s w - w)

  // low part: (c1-c7-c3+c5) z + T2*((c2-c6) + 2(c3-c5) z)
  Ciphertext s = zc3 - zc5;                 // L2
  Ciphertext u = s + s + c2 - c6;           // L2
  Ciphertext g = T2 * u;                    // L3
  Ciphertext lin = zc1 - zc7 - zc3 + zc5;   // L2

  Ciphertext y = (lin + g - w) + P + c0;
  y.set_output("y");
}
```