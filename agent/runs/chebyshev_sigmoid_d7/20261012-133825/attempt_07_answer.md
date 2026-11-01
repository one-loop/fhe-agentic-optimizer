I'm keeping the attempt-3 kernel as the final answer. Attempt 6 measured 136.90 ms, which is within the run-to-run noise of the other runs of this same structure (136.1 to 137.6 ms). Nothing I've tried has moved the time meaningfully since attempt 1.

**Why I'm stopping:** The kernel uses four ciphertext multiplications (about 30 ms each) and five plaintext multiplications. A degree-7 polynomial needs a multiplication chain of length 4 to reach its z⁷ term, so I don't see a three-multiplication schedule. Removing the `alpha` multiply would require scaling by 1/8 or 1/16 without plaintext-plaintext arithmetic, which isn't allowed. Reordering additions only shifted the time by about 1 ms, so the remaining attempts are unlikely to produce a meaningful gain.

**Final kernel (attempt 3: 136.14 ms, baseline 163.59 ms, 6 levels, max error 5.2e-08):**

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
  Plaintext c7("c7");

  // z = x / 4 in [-1, 1]  (depth 1)
  Ciphertext z = x * alpha;

  // T2 = 2 z^2 - 1  (depth 2)
  Ciphertext u = z * z;
  Ciphertext T2 = u + u - one;

  // T3 = 2 z T2 - z  (depth 3)
  Ciphertext p3 = z * T2;
  Ciphertext T3 = p3 + p3 - z;

  // T5 = 2 T2 T3 - z  (depth 4)
  Ciphertext p5 = T2 * T3;
  Ciphertext T5 = p5 + p5 - z;

  // T7 = 2 T2 T5 - T3  (depth 5)
  Ciphertext p7 = T2 * T5;
  Ciphertext T7 = p7 + p7 - T3;

  // y = c0 + c1 T1 + c3 T3 + c5 T5 + c7 T7
  Ciphertext y = z * c1;
  y = y + T3 * c3;
  y = y + T5 * c5;
  y = y + T7 * c7;
  y = y + c0;
  y.set_output("y");
}
```