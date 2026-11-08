This is a structural change: use `square()` for `w*w` and `t2*t2`. A ciphertext squaring is cheaper than a general ciphertext multiply in SEAL, and it changes the circuit's operation counts. The math is unchanged.

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
  Ciphertext w = x * alpha;
  Ciphertext u = square(w);
  Ciphertext t2 = u + u - one;
  Ciphertext s = square(t2);
  Ciphertext b = t2 * c6;
  Ciphertext g = b + b + c4;
  Ciphertext g2 = g + g;
  Ciphertext r = s * g2;
  Ciphertext low = t2 * c2 - b - g;
  r = r + low;
  r = r + w * c1;
  r = r + c0;
  r.set_output("y");
}
```