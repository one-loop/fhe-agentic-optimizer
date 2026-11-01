The best kernel is probably close to optimal already: 1 ciphertext multiply, 2 scalar plaintext multiplies, and depth 3. I don't think depth 3 can be beaten here, because the x² term needs the product c2·alpha·alpha. This variant tries a slightly different structure. It doubles z right after the first multiply (level 1), multiplies the result by c2 to get 2·c2·z directly, then adds c1 and multiplies by z. The doubling add moves from level 2 to level 1, so CHEHAB's depth/op profile may differ.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext z2 = z + z;             // 2z, level 1
  Ciphertext w = z2 * c2;            // 2*c2*z, level 2
  Ciphertext inner = w + c1;         // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod - c2 + c0;
  result.set_output("y");
}
```