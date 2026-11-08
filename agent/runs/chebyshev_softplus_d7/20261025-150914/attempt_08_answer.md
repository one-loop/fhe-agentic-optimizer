This is my last attempt and I have no structurally cheaper circuit left. Of everything I tried, attempt 2 stays the fastest at 210 ms. It has 3 ct-ct multiplies, 4 plaintext multiplies and depth 4. The variants that moved plaintext work to w or reused s (attempts 3–5, 7) were not faster. This final version only reorders where c0 is added and may well normalize to the same circuit.

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
  Ciphertext w = square(z);
  Ciphertext t2 = w + w - one;
  Ciphertext s = square(t2);
  Ciphertext t4 = s + s - one;
  Ciphertext r = t2 * c6;
  Ciphertext u = (r + r) + c4;
  Ciphertext m = t4 * u;
  Ciphertext low = (z * c1 + c0) + (t2 * c2 - r);
  Ciphertext result = low + m;
  result.set_output("y");
}
```