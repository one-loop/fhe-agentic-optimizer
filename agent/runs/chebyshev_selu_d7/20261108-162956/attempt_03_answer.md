The remaining failures come from terms landing at the same level with different multiplication histories. This time I fold each coefficient into the product itself and use identities such as c4·T4 = 2·(c4·T2)·T2 − c4. The products are:
- c4·T4 = 2·(c4·T2)·T2 − c4
- c5·T5 = 2·(c5·T2)·T3 − c5·z
- c6·T6 = 2·(c6·T3)·T3 − c6
- c7·T7 = 2·(c7·T3)·T4 − c7·z

Every high-degree term then comes out as a plaintext-scaled level-3 value times a T value, ending at level 4. That cuts the depth from the baseline's 8 to 4 or 5. I have not compiled or timed this kernel, and I can't tell in advance whether the scale histories of these level-4 terms will match.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext t2sq = square(t2);
  Ciphertext t4 = t2sq + t2sq - one;

  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext p4 = (t2 * c4) * t2;
  Ciphertext p5 = (t2 * c5) * t3;
  Ciphertext p6 = (t3 * c6) * t3;
  Ciphertext p7 = (t3 * c7) * t4;

  Ciphertext high = p4 + p5;
  high = high + p6;
  high = high + p7;
  high = high + high;
  high = high + t3 * c3;

  Ciphertext low = z * c1;
  low = low + t2 * c2;
  low = low - z * c5;
  low = low - z * c7;

  Ciphertext result = high + low;
  result = result + c0;
  result = result - c4;
  result = result - c6;
  result.set_output("y");
}
```