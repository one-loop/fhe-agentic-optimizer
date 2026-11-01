The previous attempt failed because `c6·t2·w` and `c4·t4` met at level 4 with different scale histories. I now multiply the low-order sum (c1·z + c2·t2 + c4·t4) by `one`, which costs one cheap deep-level plaintext multiplication and moves it to level 5. The T6 term stays at level 4, so CHEHAB aligns the two for free.

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
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * c6;
  Ciphertext w = t4 + t4 - one;
  Ciphertext r6 = m * w;
  Ciphertext low = z * c1;
  low = low + t2 * c2;
  low = low + t4 * c4;
  low = low * one;
  Ciphertext r = low + r6;
  r = r + c0;
  r.set_output("y");
}
```