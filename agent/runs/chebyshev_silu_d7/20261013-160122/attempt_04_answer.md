I'm stopping here with the attempt 2 kernel, which measured 214.25 ms, the best result so far (baseline 303.26 ms). Attempt 3 was the same kernel and measured 214.99 ms, which is within noise.

Further gains look marginal. Three ciphertext multiplies is the minimum for a degree-6 polynomial, and the remaining plaintext multiplies and encodings are small next to that. I don't have a change I'm confident would beat 214 ms without measuring it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Ciphertext z = x * alpha;
  Plaintext one("one");

  // T2 = 2 z^2 - 1
  Ciphertext z2 = z * z;
  Ciphertext T2 = z2 + z2 - one;

  // T4 = 2 T2^2 - 1
  Ciphertext q = T2 * T2;
  Ciphertext T4 = q + q - one;

  // c6*T6 + c4*T4 = T4 * (2 c6 T2 + c4) - c6 T2
  Plaintext c6("c6");
  Plaintext c4("c4");
  Ciphertext v = T2 * c6;
  Ciphertext u = v + v + c4;
  Ciphertext m = T4 * u;

  Plaintext c2("c2");
  Ciphertext w = T2 * c2;
  Ciphertext wv = w - v;

  Plaintext c1("c1");
  Ciphertext term1 = z * c1;

  Ciphertext result = wv + term1;
  result = result + m;
  Plaintext c0("c0");
  result = result + c0;
  result.set_output("y");
}
```