Idea: use `square()` for z·z and t2·t2. Squaring a ciphertext in SEAL needs fewer polynomial products than a general ciphertext multiplication, so it should be cheaper per operation. The math is unchanged. The rest follows the best kernel (attempt 3).

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
  Ciphertext t2 = u + u - one;
  Ciphertext u2 = square(t2);
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