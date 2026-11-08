The circuit is already at 3 ciphertext multiplications and 4 plaintext multiplications, which looks close to minimal for this basis. The remaining overhead is the 2 scale alignments and 1 mod switch. These come from adding the lower-level terms (c1·z at level 2, the T2 terms at level 3) into the level-4 product.

This attempt changes the order of the additions:

1. Add the constant c0 to c1·z first.
2. Add the T2 terms at level 3.
3. Add the level-4 product last.

Each alignment then happens on one combined lower-level term instead of separately, which may cut the alignment operations.

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

  Ciphertext z = x * alpha;            // level 1
  Ciphertext u = z * z;                // level 2
  Ciphertext t2 = u + u - one;         // level 2
  Ciphertext s = t2 * t2;              // level 3
  Ciphertext t4 = s + s - one;         // level 3

  Ciphertext c6t2 = t2 * c6;           // level 3
  Ciphertext w = c6t2 + c6t2 + c4;     // level 3
  Ciphertext high = t4 * w;            // level 4

  Ciphertext lin = z * c1 + c0;        // level 2
  Ciphertext c2t2 = t2 * c2;           // level 3
  Ciphertext low = (c2t2 - c6t2) + lin; // level 3

  Ciphertext result = low + high;
  result.set_output("y");
}
```