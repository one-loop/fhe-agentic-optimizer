I haven't found a structurally cheaper circuit, so I'm only reordering the additions in attempt 2 (low-level terms `c1·z` and `c0` first). This may normalize to the same circuit as attempt 2. The 4-level evaluation (`z`, `s`, `T4`, and the final product) is the minimum I can reach, since I can't merge the coefficients.

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
  Ciphertext s = square(z);
  Ciphertext w = z * c1;
  Ciphertext w0 = w + c0;
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a + c4;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext lower = u - a;
  Ciphertext result = lower + w0;
  result = result + p;
  result.set_output("y");
}
```