The inner sum had two separate plaintext subtractions of c7. I fold them into one by writing v = T2·q2 − c7 and inner = v + v + c3, which saves one plaintext add (~2.5 ms). Everything else is unchanged from attempt 6.

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
  Ciphertext z = x * alpha;
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext q = t2 * c7;
  Ciphertext q2 = q + q + c5;
  Ciphertext r = t2 * q2;
  Ciphertext v = r - c7;
  Ciphertext inner = v + v + c3;
  Ciphertext w = z * c1 - z * c5 - z * c7;
  Ciphertext result = t3 * inner + w;
  result.set_output("y");
}
```