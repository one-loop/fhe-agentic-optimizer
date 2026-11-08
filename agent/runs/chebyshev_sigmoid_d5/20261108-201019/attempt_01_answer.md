The baseline builds T3 and T5 with a recurrence and multiplies each by its coefficient, which needs 4 ciphertext multiplications and 6 levels. This kernel factors the polynomial instead, aiming for 3 ciphertext multiplications and 4 levels.

**Identities used.** Let u = T2 = 2z² − 1 and q3 = 2u − 1. Then:
- T3 = z·q3
- T5 = z(4u² − 2u − 1) = z(2u·q3 − 1)

**Rewritten output.** Substituting these gives

y = c0 + c1·z − c5·z + (z·q3)·(c3 + 2·c5·u)

Expanding the last product: c3·z·q3 is the c3·T3 term, and 2·c5·u·z·q3 − c5·z is the c5·T5 term.

**Expected cost.**
- Ciphertext multiplications: 3 (z², z·q3, and the final product).
- Plaintext multiplications: 4 (alpha, c5·u, c1·z, c5·z).
- Multiplicative depth: 4 instead of 6, so fewer primes and cheaper operations.
- Scales: terms with different multiplication histories meet at different levels, so no scale mismatch should occur. c1·z and c5·z share the same history.

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
  Ciphertext u = z2 + z2 - one;        // T2, level 2
  Ciphertext q3 = u + u - one;         // 2u - 1, level 2
  Ciphertext zq3 = z * q3;             // T3, level 3
  Ciphertext c5u = u * c5;             // level 3
  Ciphertext r = c5u + c5u + c3;       // c3 + 2 c5 u, level 3
  Ciphertext big = zq3 * r;            // level 4
  Ciphertext lin = z * c1 - z * c5;    // level 2
  Ciphertext result = big + lin + c0;
  result.set_output("y");
}
```