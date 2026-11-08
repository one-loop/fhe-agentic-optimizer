I found no structural change left that removes a multiplication, level, alignment or plaintext encoding.

The remaining costs look close to minimal under the CKKS rules here:
- **Depth 4:** the x⁷ term needs c7·α⁴·x⁷, which forces this depth.
- **5 ciphertext multiplies:** a 4-multiply split would need coefficient products, which require plaintext–plaintext arithmetic.
- **3 alignments:** these come from the independent linear coefficients (c1, c5−c7), which have to be lifted from level 2.

So this last attempt is a small reshuffle of the best kernel (attempt 6) and will likely change nothing measurable. `lin` is now built as `zc1 − zc7 − s`, reusing `s = zc3 − zc5`. It is folded together with −w before meeting the low product `g`, so the add tree and alignment order differ slightly.

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

  Ciphertext y = ((lin - w) + g) + P + c0;
  y.set_output("y");
}
```