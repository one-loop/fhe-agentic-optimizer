This kernel adds no ciphertext multiplication and builds T3 = 2·z·T2 − z from W = z·T2. That removes u and its plaintext add, but z has to be aligned up to level 3 once.

I then recover that aligned z for free as T1h = (W − T3) + W. It equals z, sits at level 3 and has W's scale, the same as T3. All odd-coefficient multiplications (c1, c5, c7 on z and c3, c5, c7 on T3) can then run at level 3 and land at level 4 with one shared scale pattern. Only the level-3 T2 terms need lifting.

The expected effect is about 4 scale alignments instead of 5, one fewer plaintext add, and cheaper deep-level multiplications. One risk: CHEHAB's rewriter may simplify (W − T3) + W back to z, which would land close to attempt 1.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext one("one");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Plaintext c3("c3");
  Plaintext c4("c4");
  Plaintext c5("c5");
  Plaintext c6("c6");
  Plaintext c7("c7");

  Ciphertext T1 = x * alpha;
  Ciphertext zz = square(T1);
  Ciphertext T2 = zz + zz - one;
  Ciphertext t22 = square(T2);

  Ciphertext W = T1 * T2;
  Ciphertext T3 = W + W - T1;
  Ciphertext T1h = (W - T3) + W;

  // level-4 odd terms (same scale pattern)
  Ciphertext a5 = T1h * c5;
  Ciphertext a7 = T3 * c7;
  Ciphertext l1 = T1h * c1 - T1h * c7;
  Ciphertext l3 = T3 * c3 - T3 * c5;

  // level-3 even terms
  Ciphertext a6 = T2 * c6;
  Ciphertext l2 = T2 * c2 - a6;

  Ciphertext q = (a7 + a5) + a6;
  Ciphertext inner = q + q + c4;

  Ciphertext P = t22 * inner;

  Ciphertext low = (l3 + l1) + l2;
  Ciphertext rest = low - inner;
  Ciphertext result = P + P + rest;
  result = result + c0;
  result.set_output("y");
}
```