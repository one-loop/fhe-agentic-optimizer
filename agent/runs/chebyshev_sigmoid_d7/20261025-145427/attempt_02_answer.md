Attempt 1 failed on a scale mismatch between the c5 and c7 terms, which had different multiplication histories. This version builds T5 as t2·t3 and T7 as t3·t4. Both then sit at depth 4 with the same product pattern, and they are added together before meeting the shallower terms. It uses 5 ct-ct multiplies instead of 6.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext s = square(z);
  Ciphertext t2 = s + s - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext t2s = square(t2);
  Ciphertext t4 = t2s + t2s - one;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext p7 = t3 * t4;
  Ciphertext t7 = p7 + p7 - z;
  Ciphertext hi = t5 * c5 + t7 * c7;
  Ciphertext lo = z * c1 + t3 * c3;
  Ciphertext result = hi + lo;
  result = result + c0;
  result.set_output("y");
}
```