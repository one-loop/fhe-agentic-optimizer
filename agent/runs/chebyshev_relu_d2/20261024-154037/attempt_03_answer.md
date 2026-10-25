Every variant I can find that is structurally different from the best costs an extra plaintext multiplication, so it is likely slower. For this attempt I'm trying a square-based form, y = (c0 − c2) + c1·z + 2c2·z², where the square replaces the ciphertext-ciphertext multiply. It has 3 plaintext multiplications, so I expect it to lose, but it gives a genuinely different circuit to measure.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext q = z2 * c2;
  Ciphertext l = z * c1;
  Ciphertext r = (q + q) + l;
  Ciphertext y = (r - c2) + c0;
  y.set_output("y");
}
```