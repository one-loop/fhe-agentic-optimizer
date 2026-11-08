The 3 ciphertext multiplications are already the minimum for a degree-5 polynomial. Attempt 2 was slowed by needing 5 levels, because `c5` was multiplied onto u only at level 2→3.

This attempt applies `c5` to x at level 0, in parallel with `alpha`. Then (c5·x)·z = 4·c5·z² = c5·(q3 + 3) is ready at level 2. So r = c5·q3 + c3 + c5 is computed as (c5x·z) + c3 − c5 − c5, also at level 2.

The rest follows attempt 2: inner = q3·r + c1 − c5 at level 3, and y = z·inner + c0 at level 4. The circuit has depth 4 and only 2 plaintext multiplications, at the cost of one extra ciphertext multiplication done at a cheap low level. I expect this extra multiplication to cost more than the level and plaintext multiplications it saves, so this may not beat attempts 1 and 2. I'm running it mainly to measure that trade-off.

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
  Ciphertext s = x * c5;               // level 1  (= 4 c5 z)
  Ciphertext z2 = z * z;               // level 2
  Ciphertext u = z2 + z2 - one;        // T2
  Ciphertext q3 = u + u - one;         // 4z^2 - 3
  Ciphertext t = s * z;                // level 2, = 4 c5 z^2 = c5 (q3 + 3)
  Ciphertext r = t + c3 - c5 - c5;     // c5 q3 + c3 + c5
  Ciphertext inner = q3 * r + c1 - c5; // level 3
  Ciphertext result = z * inner + c0;  // level 4
  result.set_output("y");
}
```