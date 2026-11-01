No new circuit this time: I resubmit attempt 1 unchanged, because I couldn't find a cheaper structure. It will be reported as identical and not timed.

Every option I tried gives the same circuit or a more expensive one:

- **Depth 3 is the floor.** The x² term needs c2·alpha·alpha, and the plaintexts can't be multiplied together, so each factor costs its own level.
- **Two plaintext multiplies is the floor.** Every variant that also uses x·c2 or z·c1, or squares z first, needs at least 3.
- **The constant terms are already minimal.** c1, c0 and c2 each enter through one plaintext add, and no plaintext-only combination like c0 − c2 is allowed.
- **The ciphertext multiply is already at the cheap level.** It runs at level 2.
- **The mod switch can't be removed.** It is a ~1 ms alignment of z to level 2.
- **The doubling has no cheaper form.** Writing 2·c2·z as `w + w` is a 0.5 ms add, and removing it would need an extra multiply.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;          // level 1
  Ciphertext w = z * c2;             // level 2: c2*z
  Ciphertext inner = w + w + c1;     // 2*c2*z + c1
  Ciphertext prod = inner * z;       // level 3
  Ciphertext result = prod + c0 - c2;
  result.set_output("y");
}
```