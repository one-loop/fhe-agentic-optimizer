This is the same kernel again. The differences between attempts 3 to 7 (212–214 ms) are run-to-run timing noise, since the operation counts are identical. Attempt 6 holds the best time at 212.22 ms, and I found no rewrite that lowers the operation counts further.

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
  Ciphertext t4 = s + s - one;
  Ciphertext m = t2 * c6;
  Ciphertext q = m + m + c4;
  Ciphertext p = t4 * q;
  Ciphertext result = z * c1;
  result = result + t2 * c2;
  result = result - m;
  result = result + p;
  result = result + c0;
  result.set_output("y");
}
```