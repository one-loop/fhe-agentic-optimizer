Last try: I replace the four plaintext additions of c2 and 3·c6 inside the Horner step with one plaintext multiplication, c2·t2, and a ciphertext subtraction of 3w, where w = c6·t2 is already computed. That should save a few milliseconds of plaintext encoding while keeping the same level count.

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
  Ciphertext t2 = u + u - one;
  Ciphertext w = t2 * c6;
  Ciphertext w2 = w + w;
  Ciphertext w3 = w2 + w;
  Ciphertext inner = w2 + c4;
  Ciphertext m = inner * t2;
  m = m + m;
  Ciphertext r = m * t2;
  Ciphertext b = t2 * c2;
  Ciphertext lo = b - w3;
  Ciphertext result = r + lo;
  result = result + z * c1;
  result = result + c0;
  result = result - c4;
  result.set_output("y");
}
```