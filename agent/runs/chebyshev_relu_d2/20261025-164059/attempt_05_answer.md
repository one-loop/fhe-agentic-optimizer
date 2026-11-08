Attempt 1's operation count looks minimal for this form, so I'm trying a different structure. It forms the product (x·alpha)·(x·c2) in parallel, then fixes the scale with one more `alpha`. This probably costs more, but it is a genuinely different circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext b = x * c2;
  Ciphertext p = z * b;
  Ciphertext p2 = p + p;
  Ciphertext t = p2 * alpha;
  Ciphertext a = z * c1;
  Ciphertext r = t + a;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```