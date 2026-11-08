This is the last attempt, and I keep the attempt 6 ordering. The only change is to use a plain multiply for z² and keep `square()` for T2². That mix has not been timed yet.

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
  Ciphertext u = z * z;
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext ms = m * s;
  Ciphertext d = t2 * c2 - m - a;
  Ciphertext w = z * c1;
  Ciphertext result = ((ms + ms) + d) + w;
  result = result + c0;
  result.set_output("y");
}
```