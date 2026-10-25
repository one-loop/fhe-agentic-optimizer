This is the same kernel as attempts 4 and 5, and 212.6 ms is the best time so far. I could not find a change that would remove a ct-ct multiply, a plaintext multiply, or a level without plaintext-plaintext arithmetic, so I'm not spending the remaining attempts on guesses.

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
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * c6;
  Ciphertext q = m + m + c4;
  Ciphertext p = t4 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - m;
  result = result + p;
  result = result + c0;
  result.set_output("y");
}
```