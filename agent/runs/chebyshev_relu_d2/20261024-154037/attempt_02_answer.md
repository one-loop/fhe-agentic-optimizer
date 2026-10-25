I could not find a circuit with fewer operations than the current best. It uses the minimum of 2 plaintext multiplications and 1 ciphertext multiplication, and every alternative I tried costs another plaintext multiplication or an extra level. This attempt keeps the same factorization, y = (c0 − c2) + z·(c1 + 2c2·z), and only reorders how the constant terms are added. CHEHAB will likely normalize that away, so I do not expect a measurable gain.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext m = z * c2;
  Ciphertext s = (m + m) + c1;
  Ciphertext p = s * z;
  Ciphertext r = p - c2;
  Ciphertext y = r + c0;
  y.set_output("y");
}
```