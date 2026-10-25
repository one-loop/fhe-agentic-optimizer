The level-4 mismatch was probably `t3*c3 + t4*c4`: `t3` and `t4` were built differently (one contains `-z`, the other `-one`). This version changes the construction so the one colliding pair has the same form:

- `t3 = 2·z·t2 − z`
- `t4 = 2·z·t3 − t2` and `t5 = 2·t2·t3 − z`, both a ciphertext product minus a lower ciphertext
- `t6 = 2·z·t5 − t4`
- `t7 = 2·z·t6 − t5`

Term levels are then 2, 3, 4, 5 (`t4` and `t5` together), 6 and 7.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");

  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext zt3 = z * t3;
  Ciphertext t4 = zt3 + zt3 - t2;
  Ciphertext p25 = t2 * t3;
  Ciphertext t5 = p25 + p25 - z;
  Ciphertext zt5 = z * t5;
  Ciphertext t6 = zt5 + zt5 - t4;
  Ciphertext zt6 = z * t6;
  Ciphertext t7 = zt6 + zt6 - t5;

  Ciphertext r = z * c1;
  r = r + t2 * c2;
  r = r + t3 * c3;
  Ciphertext g = t4 * c4 + t5 * c5;
  r = r + g;
  r = r + t6 * c6;
  r = r + t7 * c7;
  r = r + c0;
  r.set_output("y");
}
```