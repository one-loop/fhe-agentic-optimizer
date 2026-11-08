Idea: double u once, up front, instead of doubling the product ub afterwards (u2 = u + u, then w = u2·b + c2). This should shave an add from the critical path while keeping the 3 ct-ct and 3 ct-pt multiplications.

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
  Ciphertext u = s + s - one;
  Ciphertext u2 = u + u;
  Ciphertext a = u * c6;
  Ciphertext a2 = a + a;
  Ciphertext b = a2 + c4;
  Ciphertext w = (u2 * b) + c2;
  Ciphertext r = u * w;
  r = r + z * c1;
  r = r - (a2 + a);
  r = r + c0;
  r = r - c4;
  r.set_output("y");
}
```