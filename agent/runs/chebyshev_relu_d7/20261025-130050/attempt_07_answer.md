Idea: factor T4 out of the c4 and c6 terms. T6 = t·(2·T4 − 1), so the result is

y = T4·(2·c6·t + c4) + (c2·t − c6·t) + c1·z + c0.

This keeps depth at 4 with 3 ct-ct multiplications and 4 plaintext multiplications, and needs only 4 plaintext additions (−one twice, +c4, +c0).

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
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext t4 = s + s - one;
  Ciphertext tc6 = t * c6;
  Ciphertext g = tc6 + tc6 + c4;
  Ciphertext low = t * c2 - tc6 + z * c1 + c0;
  Ciphertext result = t4 * g + low;
  result.set_output("y");
}
```