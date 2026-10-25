Fix: merge the c4 and c6 terms into one product, so that the sum that meets it sits at a different level. Since c4·T4 + 2·c6·T2·T4 = T4·(c4 + 2·(c6·T2)), the c4 plaintext multiplication disappears. That leaves 4 plaintext multiplications and 3 ciphertext multiplications. The remaining terms (c2·T2, c1·z, −c6·T2) all sit at levels below the level-4 product, so CHEHAB can align them without an extra level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c4("c4");
  Plaintext c6("c6");
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext p = t4 * m;
  Ciphertext low = t2 * c2 + z * c1;
  low = low - a6;
  Ciphertext result = p + low;
  result = result + c0;
  result.set_output("y");
}
```