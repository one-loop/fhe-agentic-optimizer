The last idea saves one plaintext subtraction (about 2.5 ms). With u = z², T2 = 2u − 1 is one doubling plus one subtraction. Then s−2 = 2·T2 is a ciphertext add, and s−3 needs one more subtraction. That is two plaintext subtractions instead of three, in exchange for one cheap ciphertext add. I haven't timed it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");
  Ciphertext z = x * alpha;
  Ciphertext u = square(z);
  Ciphertext u2 = u + u;
  Ciphertext t2 = u2 - one;
  Ciphertext s2 = t2 + t2;
  Ciphertext s3 = s2 - one;
  Ciphertext b = s2 * c5;
  b = b + c3;
  Ciphertext g = s3 * b;
  g = g + c1;
  g = g - c5;
  Ciphertext y = z * g;
  y = y + c0;
  y.set_output("y");
}
```