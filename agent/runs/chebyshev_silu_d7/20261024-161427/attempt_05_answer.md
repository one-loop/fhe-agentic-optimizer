Idea: use `square()` for u = z² and s = t2². Squaring is cheaper than a general ciphertext product in SEAL, and the rest of the circuit stays as in attempt 3.

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