Attempts 2 and 3 both hit scale mismatches at level 4, so I'm going back to the working attempt-1 structure, where the deep terms land at different levels. The one change is to simplify T6. Since T6 = 2·w·T4 − w = w·(2·T4 − 1), a single ct·ct multiply replaces the multiply plus subtract. The ct–ct multiplies stay at three, and the c6 term still lands at level 5, apart from the c4 term at level 4.

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
  Ciphertext w = u + u - one;
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext s = t4 + t4 - one;
  Ciphertext t6 = w * s;
  Ciphertext result = z * c1;
  result = result + w * c2;
  result = result + t4 * c4;
  result = result + t6 * c6;
  result = result + c0;
  result.set_output("y");
}
```