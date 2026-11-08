Attempt 7 failed because t3·c6 and t4·c7 meet at the same level with different multiplication histories. Every fix I found adds a ciphertext multiplication, which would be slower than the current best. For the last attempt I keep the attempt-6 structure and make a small rewrite. It writes t2·c2 as (z2+z2)·c2 − c2, which removes one `t2` formation from the c2 term. It may not beat the best by the 3% margin.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c2("c2"), c3("c3"), c4("c4"), c5("c5"), c6("c6"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext u = t2 + t2 - one;
  Ciphertext t3 = z * u;
  Ciphertext m4 = z * t3;
  Ciphertext t4 = m4 + m4 - t2;

  Ciphertext cz5 = z * c5;
  Ciphertext g = (cz5 + cz5) + c4;
  Ciphertext B = t4 * g;

  Ciphertext w = t3 * c6 + t4 * c7;
  Ciphertext v = w + w + c3 - c5;
  Ciphertext C = t3 * v - c6 - z * c7;

  Ciphertext result = z * c1 + (z2 + z2) * c2;
  result = result - c2;
  result = result + B;
  result = result + C;
  result = result + c0;
  result.set_output("y");
}
```