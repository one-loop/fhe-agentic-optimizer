This is the last attempt, and it is a small reordering of attempt 3. The plaintext constants (−c7 and c1) are added to the c5·e5 term at level 4, before it meets the level-5 product e3·w. Adding them at a lower level is slightly cheaper, but the operation counts are probably unchanged, so it may not count as a new circuit.

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
  Plaintext c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext t = u + u - one;
  Ciphertext e3 = t + t - one;
  Ciphertext t4 = square(t);
  t4 = t4 + t4 - one;
  Ciphertext e5 = t4 + t4 - e3;
  Ciphertext q = t4 * c7;
  Ciphertext w = q + q + c3;
  Ciphertext r = e5 * c5;
  r = r - c7;
  r = r + c1;
  Ciphertext s = e3 * w + r;
  Ciphertext result = z * s + c0;
  result.set_output("y");
}
```