The previous attempt added the c3·T3 term and the c5 term at the same level (4) with different multiplication histories. This time I apply c3 before the ct-ct multiply, using c3·T3 = 2·(z·c3)·t2 − z·c3. That puts the c3 term at level 3, so it no longer meets the c5 term at the same level.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Plaintext c7("c7");
  Plaintext c0("c0");
  Ciphertext z = x * alpha;
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext a3 = z * t2;
  Ciphertext t3 = a3 + a3 - z;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a7 = t3 * t4;
  Ciphertext t7 = a7 + a7 - z;
  Ciphertext zc3 = z * c3;
  Ciphertext u3 = zc3 * t2;
  Ciphertext term3 = u3 + u3 - zc3;
  Ciphertext u5 = (t2 * c5) * t3;
  Ciphertext term5 = u5 + u5 - z * c5;
  Ciphertext term7 = t7 * c7;
  Ciphertext low = z * c1 + term3;
  Ciphertext result = term7 + (term5 + low);
  result = result + c0;
  result.set_output("y");
}
```