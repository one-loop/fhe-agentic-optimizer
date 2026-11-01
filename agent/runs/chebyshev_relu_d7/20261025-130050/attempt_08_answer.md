This is the last attempt, so I make a small variation on attempt 6, the fastest kernel so far. Instead of forming h = 2·T4 − 1 as a ciphertext, I apply c6 to 2·T4 and subtract c6 afterwards as a plaintext. This swaps the order of the plaintext operations, and may let CHEHAB schedule levels and scales differently.

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
  Ciphertext t = u + u - one;
  Ciphertext s = square(t);
  Ciphertext t4 = s + s - one;
  Ciphertext w = t4 + t4;
  Ciphertext inner = w * c6 - c6 + c2;
  Ciphertext result = t * inner + t4 * c4 + z * c1 + c0;
  result.set_output("y");
}
```