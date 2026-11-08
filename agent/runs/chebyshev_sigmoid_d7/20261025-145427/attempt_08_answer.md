The last result shows that fewer adds and fewer plaintext multiplies help. This version also cuts depth by pushing the c5 and c7 multiplies earlier:
- c5·B5 = (c5·b)·B3 − c5
- c7·B7 = (c7·b)·B5 − c7·b + c7

This would reduce the final depth from 6 to 5. If the two depth-4 terms report a scale mismatch, attempt 7 remains the best.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0"), c1("c1"), c3("c3"), c5("c5"), c7("c7");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext a = u + u - one;
  Ciphertext b = a + a;
  Ciphertext b3 = b - one;
  Ciphertext p5 = b * b3;
  Ciphertext b5 = p5 - one;

  Ciphertext cb5 = b * c5;
  Ciphertext cb7 = b * c7;
  Ciphertext h5 = cb5 * b3;
  Ciphertext h7 = cb7 * b5;

  Ciphertext q = b3 * c3;
  Ciphertext hi = (h5 - c5) + ((h7 - cb7) + c7);
  q = q + hi;
  q = q + c1;
  Ciphertext r = z * q;
  r = r + c0;
  r.set_output("y");
}
```