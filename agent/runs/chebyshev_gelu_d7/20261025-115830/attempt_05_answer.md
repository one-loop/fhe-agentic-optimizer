Idea: this is the same circuit as attempt 4 (depth 4) with one plaintext add removed. Instead of adding c4 twice, I form (2t + c4) once and double it. A plaintext add costs about 2.5 ms because of encoding, while a ciphertext add costs about 0.5 ms.

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
  Ciphertext w = u + u - one;
  Ciphertext s = square(w);
  Ciphertext t = w * c6;
  Ciphertext t2 = t + t;
  Ciphertext k0 = t2 + c4;
  Ciphertext k = k0 + k0;
  Ciphertext f = s * k;
  Ciphertext m = w * c2;
  m = m - t;
  m = m - t;
  m = m - t;
  m = m - c4;
  m = m + c0;
  Ciphertext g = z * c1;
  m = m + g;
  Ciphertext result = f + m;
  result.set_output("y");
}
```