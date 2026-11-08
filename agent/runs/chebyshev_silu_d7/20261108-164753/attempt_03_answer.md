The previous kernel is correct and fast, so I make a smaller change. Writing `m*t4` as `2*m*s - m` (with `s = T2²`) drops the separate `T4 = 2s - 1` and its plaintext subtraction. I also fold the `- m` term into the lower-level sum. The ciphertext multiplications and levels stay the same. This is a minor saving and I have not run it.

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
  Ciphertext s = t2 * t2;
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext ms = m * s;
  Ciphertext d = t2 * c2 - m;
  Ciphertext w = z * c1 + c0;
  Ciphertext result = (ms + ms) + (d + w);
  result.set_output("y");
}
```