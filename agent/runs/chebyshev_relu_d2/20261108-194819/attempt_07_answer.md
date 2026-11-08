No new idea this round. I could not find a circuit cheaper than attempt 1:

- **Fixed costs:** the depth of 3, the single ciphertext×ciphertext multiply, the 2 ciphertext×plaintext multiplies and the 4 additions are each at their minimum for this polynomial under the CKKS rules (no constants, no plaintext-plaintext arithmetic).
- **Remaining overhead:** the only extra operation is a ~1 ms mod switch. Every alternative I checked adds at least one ~5 ms plaintext multiply, or adds a level.

The kernel below is attempt 1 unchanged, so it will come back as identical to the best and not be timed. That makes it a wasted attempt, but I would rather not spend the last ones on circuits I expect to be slower. The final result is 17.0 ms, against 24.0 ms for the baseline (about 1.4×).

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext v = w + w + c1;
  Ciphertext p = z * v;
  Ciphertext y = p - c2 + c0;
  y.set_output("y");
}
```