Idea: use t6 = 2·t2·t4 − t2 to fold the c4 and c6 terms into one product: c4·t4 + c6·t6 = t4·(c4 + 2·c6·t2) − c6·t2. The product c6·t2 is reused for both the doubling and the subtraction. This removes one plaintext multiplication and cuts the depth from 5 to 4. The ciphertext multiplication count stays at 3.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext s2 = square(t2);
  Ciphertext t4 = s2 + s2 - one;
  Ciphertext a = t2 * c6;
  Ciphertext q = a + a + c4;
  Ciphertext r = t4 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + r;
  result = result - a;
  result = result + c0;
  result.set_output("y");
}
```