Idea: t4 is only used in the product t4·u. Since t4 = 2s − 1, that product equals s·(u+u) − u. Rewriting it this way removes t4 and one plaintext add (the encode costs about 2.5 ms), at the price of one cheap ciphertext subtract. The multiplies and depth are unchanged.

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
  Ciphertext r = t2 * c6;
  Ciphertext u = (r + r) + c4;
  Ciphertext m = s * (u + u) - u;
  Ciphertext result = z * c1 + t2 * c2 - r + m + c0;
  result.set_output("y");
}
```