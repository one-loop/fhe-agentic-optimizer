I don't have a further circuit reduction that is safe, so I'm returning the current best kernel. It is the attempt-3 kernel, with the same circuit, so it should not displace the current best.

The remaining ideas would need plaintext-plaintext arithmetic, such as precomputing alpha² or combining coefficients, which CKKS rules out here. Others only reshuffle the same operation counts.

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
  Ciphertext u2 = t2 * t2;
  Ciphertext t4 = u2 + u2 - one;
  Ciphertext a6 = t2 * c6;
  Ciphertext m = a6 + a6 + c4;
  Ciphertext p = t4 * m;
  Ciphertext low = t2 * c2 + z * c1;
  low = low - a6;
  Ciphertext result = p + low;
  result = result + c0;
  result.set_output("y");
}
```