I'm resubmitting attempt 2's kernel unchanged. Attempts 3 to 5 did not beat it: attempt 3 compiled to the identical circuit, and attempts 4 and 5 were new circuits that timed slower (215 ms and 220 ms vs 213 ms).

It remains the best circuit I have: 3 ciphertext multiplications (two of them squares), 4 plaintext multiplications and 4 levels.

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
  Ciphertext t2 = s + s - one;
  Ciphertext q = square(t2);
  Ciphertext t4 = q + q - one;
  Ciphertext a = t2 * c6;
  Ciphertext a2 = a + a + c4;
  Ciphertext p = a2 * t4;
  Ciphertext u = t2 * c2;
  Ciphertext w = z * c1;
  Ciphertext lower = u - a;
  Ciphertext result = p + lower;
  result = result + w;
  result = result + c0;
  result.set_output("y");
}
```