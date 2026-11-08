I don't see a way to reduce the operation counts below those of attempt 2. This attempt is a variant of the best kernel that doubles z at level 1 with a cheap add before the c2 product, instead of doubling after it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext zz = z + z;
  Ciphertext g2 = zz * c2;
  Ciphertext m = g2 + c1;
  Ciphertext r = m * z;
  r = r + c0;
  r = r - c2;
  r.set_output("y");
}
```