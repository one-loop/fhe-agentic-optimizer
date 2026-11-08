The structure of attempt 4 seems close to optimal under these rules. I don't see a way to get 3 ct-ct multiplications at depth 4 with fewer than 4 plaintext multiplications, and depth 5 with 3 ct-ct and 2 plaintext multiplications is what attempt 4 uses. That leaves only small variations.

This attempt applies `c5` to q3 instead of u, using r = c5·q3 + c3 + c5 (since 2u = q3 + 1). Two ciphertext additions (c5u + c5u) become one plaintext add of c5. The dependency chain also changes, so CHEHAB may schedule the alignments differently. Any gain will be small, if there is one.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c3("c3");
  Plaintext c5("c5");

  Ciphertext z = x * alpha;            // level 1
  Ciphertext z2 = square(z);           // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 2u - 1
  Ciphertext r = q3 * c5 + c3 + c5;    // c3 + 2 c5 u, level 3
  Ciphertext inner = q3 * r + c1 - c5; // level 4
  Ciphertext result = z * inner + c0;  // level 5
  result.set_output("y");
}
```