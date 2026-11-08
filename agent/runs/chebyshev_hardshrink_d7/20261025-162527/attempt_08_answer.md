For the last attempt I'm using the lowest-depth structure from attempt 3, with c5 + 2·c7·T2 factored out of the T5 and T7 terms, and `square` for z². This gives a circuit that has not been tried yet.

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
  Ciphertext z2 = square(z);
  Ciphertext t2 = z2 + z2 - one;
  Ciphertext zt2 = z * t2;
  Ciphertext t3 = zt2 + zt2 - z;
  Ciphertext p5 = t2 * t3;
  Ciphertext t5 = p5 + p5 - z;
  Ciphertext m = t2 * c7;
  Ciphertext w = m + m + c5;
  Ciphertext result = z * c1;
  result = result + t3 * c3;
  result = result - t3 * c7;
  result = result + t5 * w;
  result.set_output("y");
}
```