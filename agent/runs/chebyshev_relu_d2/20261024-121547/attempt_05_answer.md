I'm submitting the attempt 3 kernel again, because the other forms I considered cost more. The `(2z² − 1)` form with `one` needs three ct-pt multiplies, like the baseline. The operation counts (one ct-ct multiply, two ct-pt multiplies, three plaintext adds) are already at the minimum I can reach, and the time differences between attempts 1 to 4 are measurement noise.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext s = w + c1;
  s = s + w;
  Ciphertext r = z * s;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```