This is a modest variation on the best kernel. I fold `c4` into the doubled inner sum differently and add the lower part in a different order. This may or may not change CHEHAB's circuit. It keeps the 4-ct-ct-multiply structure around T4 and shares `t2*c6` between the inner and lower parts.

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
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s4 = square(t2);
  Ciphertext t4 = s4 + s4 - one;

  Ciphertext p6 = t2 * c6;
  Ciphertext q = z * c5 + p6;
  q = q + t3 * c7;
  Ciphertext w = q + q + c4;
  Ciphertext prod = t4 * w;

  Ciphertext low = (z * c1 + t2 * c2) + t3 * c3;
  Ciphertext sub = (z * c7 + p6) + t3 * c5;
  low = low - sub;
  low = low + c0;
  Ciphertext result = prod + low;
  result.set_output("y");
}
```