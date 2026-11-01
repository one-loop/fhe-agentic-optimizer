I am resubmitting attempt 1 unchanged, because I believe it is already the cheapest circuit for this task. The evaluator will report it as identical and not time it.

These were the realistic alternatives, and each is slower:

- **Multiply by x, apply alpha last.** This removes the 1 ms mod switch on z. It adds a third plaintext multiply (~3–5 ms) and a fourth level, so the net cost is higher.
- **Square first, or compute x·c2 or z·c1 separately.** Each of these needs at least 3 plaintext multiplies instead of 2.
- **Combine the constants beforehand.** Forming c0 − c2 or c1/(2·c2) as one plaintext is not allowed (no plaintext-plaintext arithmetic, no numeric constants). So the three constant adds stay.
- **Lower the depth.** Depth 3 is the minimum, because the x² term needs the product c2·alpha·alpha, and each plaintext factor costs one level.

The current kernel has 1 ciphertext×ciphertext multiply at level 2 and 2 scalar plaintext multiplies. It also has 3 plaintext adds, 1 ciphertext add and 1 mod switch. Each of those counts is already at its minimum.

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