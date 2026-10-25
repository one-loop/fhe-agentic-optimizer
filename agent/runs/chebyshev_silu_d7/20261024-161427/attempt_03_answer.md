Idea: reuse t4 = 2s − 1 to get 4s − 3 as 2·t4 − 1. That needs two plaintext subtractions of `one` instead of four, each costing about 2.5 ms of encoding, with the same multiplication structure.

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
  Ciphertext s2 = s + s;
  Ciphertext t4 = s2 - one;
  Ciphertext q = t4 + t4 - one;
  Ciphertext w = q * c6 + c2;
  Ciphertext r = t2 * w;
  Ciphertext result = z * c1;
  result = result + t4 * c4;
  result = result + r;
  result = result + c0;
  result.set_output("y");
}
```