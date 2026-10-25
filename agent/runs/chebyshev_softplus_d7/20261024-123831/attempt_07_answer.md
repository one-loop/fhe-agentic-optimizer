I have no further improvement that I trust, so this is the best kernel from attempt 6 unchanged (221.76 ms, down from the 313.32 ms baseline). It computes the even part as a polynomial in w = T2 and reaches depth 5 with three ct–ct multiplies and four ct–pt multiplies. The remaining ideas I considered would add ct–ct or plaintext-multiply work, or risk the level-4 scale mismatches from attempts 2 and 3.

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
  Ciphertext w2 = square(w);
  Ciphertext t4 = w2 + w2 - one;
  Ciphertext sp = t4 + t4 - one;
  Ciphertext a = w * c4;
  Ciphertext inner = sp * c6;
  inner = inner + a + a;
  inner = inner + c2;
  Ciphertext result = w * inner;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```