I have no further circuit reduction to offer. Attempts 3–5 changed the circuit but were not faster, so attempt 2 is still the best: 3 ct-ct multiplies, 4 plaintext multiplies and depth 4. I'm resubmitting it unchanged.

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
  Ciphertext result = z * c1 + t2 * c2 - r + m + c0;
  result.set_output("y");
}
```