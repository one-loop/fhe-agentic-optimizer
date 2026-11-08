This version cuts the depth from 5 to 4. It computes y = (z·(s−3))·b + z·c1 − z·c5 + c0, so the multiply by z overlaps with b. That costs two extra plaintext multiplies, but it takes one level off the critical path. I haven't timed it, so it may not pay off.

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
  Ciphertext s = u2 + u2;
  Ciphertext s2 = s - one;
  s2 = s2 - one;
  Ciphertext s3 = s2 - one;
  Ciphertext b = s2 * c5;
  b = b + c3;
  Ciphertext zs = z * s3;
  Ciphertext y = zs * b;
  Ciphertext t1 = z * c1;
  Ciphertext t5 = z * c5;
  y = y + t1;
  y = y - t5;
  y = y + c0;
  y.set_output("y");
}
```