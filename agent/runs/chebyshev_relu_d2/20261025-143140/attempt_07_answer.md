I could not find a circuit with fewer operations than attempt 1, and each structural change so far has added a plaintext multiplication. This attempt adds the constants c0 and −c2 in the opposite grouping and builds the factor from c1 first. CHEHAB may reduce it to the same circuit as the best, but it is the only distinct arrangement left that does not add operations.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = c2 * z;
  Ciphertext u = c1 + (w + w);
  Ciphertext p = u * z;
  Ciphertext y = c0 + (p - c2);
  y.set_output("y");
}
```