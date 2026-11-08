The 5th-degree polynomial already uses the minimum of 3 ciphertext multiplications. This attempt cuts the scalar plaintext multiplications from 4 to 2, at the cost of one more level (5 instead of 4).

The idea is to factor out z completely:

y = c0 + z·(q3·r + c1 − c5), with u = 2z² − 1, q3 = 2u − 1 and r = c3 + 2·c5·u.

Here c1 and c5 enter only as plaintext adds, so `z*c1` and `z*c5` are no longer needed. The remaining plaintext multiplications are `alpha` and `c5·u`.

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
  Ciphertext z2 = z * z;               // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 2u - 1
  Ciphertext c5u = u * c5;             // level 3
  Ciphertext r = c5u + c5u + c3;       // c3 + 2 c5 u
  Ciphertext inner = q3 * r + c1 - c5; // level 4
  Ciphertext result = z * inner + c0;  // level 5
  result.set_output("y");
}
```