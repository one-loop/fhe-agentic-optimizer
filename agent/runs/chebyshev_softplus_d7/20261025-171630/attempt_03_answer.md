Idea: Horner-style evaluation in t2 = T2. The even part is c0 − c4 + (c2 − 3c6)·t + 2c4·t² + 4c6·t³, which equals c0 − c4 + c2·t − 3a + t²·(2c4 + 4a) with a = c6·t. This drops t4 and its extra adds and subtracts. The multiplication count is unchanged: still 3 ciphertext multiplications and 4 plaintext multiplications.

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
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a;
  Ciphertext q = a2 + c4;
  Ciphertext r = s2 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result + r + r;
  result = result - a2 - a;
  result = result - c4;
  result = result + c0;
  result.set_output("y");
}
```