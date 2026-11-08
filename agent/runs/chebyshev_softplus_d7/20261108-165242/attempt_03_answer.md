The idea here is to apply c2 to u instead of s, using c2·s = 2·c2·u − c2. Then c2·u sits at the same level as c1·z, so those two terms combine without a scale alignment, and the c2 correction is a cheap plaintext subtraction.

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
  Ciphertext s = u + u - one;
  Ciphertext q = square(s);
  Ciphertext t4 = q + q - one;
  Ciphertext c6s = s * c6;
  Ciphertext d = c6s + c6s + c4;
  Ciphertext c2u = u * c2;
  Ciphertext lin = z * c1;
  Ciphertext early = c2u + c2u + lin;
  Ciphertext main_term = t4 * d;
  Ciphertext result = main_term + early - c6s;
  result = result - c2 + c0;
  result.set_output("y");
}
```