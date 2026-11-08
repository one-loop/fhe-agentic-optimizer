Attempt 3 was wrong because I dropped a term. Expanding `m*t4 = 2ms - m` leaves the old `-a` from `d` still needed, so the low-level sum must be `c2*T2 - m - a`. I restore that term and keep the rest of attempt 3.

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
  Ciphertext a = t2 * c6;
  Ciphertext m = a + a + c4;
  Ciphertext ms = m * s;
  Ciphertext d = t2 * c2 - m - a;
  Ciphertext w = z * c1 + c0;
  Ciphertext result = (ms + ms) + (d + w);
  result.set_output("y");
}
```