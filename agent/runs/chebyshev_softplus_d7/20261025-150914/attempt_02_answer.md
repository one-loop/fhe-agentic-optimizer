Idea: fold the c6 term into the t4 multiplication, so c4 and c6 no longer need their own plaintext multiplies of t4 and t6.

Since T6 = 2·t2·t4 − t2, we get c6·T6 = t4·(2·c6·t2) − c6·t2. Let r = t2·c6. Then

y = c1·z + c2·t2 − r + t4·(c4 + r + r) + c0

Adding the plaintext c4 to a ciphertext is allowed. This needs 3 plaintext multiplies (c1, c2, c6) instead of 5, and it removes the separate t6 computation. The ct-ct multiplies stay at 3, and the depth drops from 5 to 4.

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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext r = t2 * c6;
  Ciphertext u = (r + r) + c4;
  Ciphertext m = t4 * u;
  Ciphertext result = z * c1 + t2 * c2 - r + m + c0;
  result.set_output("y");
}
```