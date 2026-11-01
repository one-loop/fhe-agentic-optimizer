I'm stopping here rather than spending the remaining attempts on a kernel I expect to be no faster. The best measured kernel is attempt 1 (also submitted as attempts 3 and 4), at 317.94 ms against a 318.50 ms baseline, a 0.2% gain that is within run-to-run noise. Its operation counts match the baseline exactly.

Why I don't expect further gains from restructuring the polynomial evaluation:

- **Depth budget.** With 10 levels, the final `x*h` takes one, so each of the three stages gets depth 3. Within that, a degree-7 odd polynomial needs 5 ciphertext multiplies. Getting to 4 forces depth 4 per stage, which does not fit.
- **Operation counts.** The rewrites I tried (attempt 2 moved the `half*x` term and was slower at 327 ms; attempt 1 reordered the sum) either left the ciphertext multiply count unchanged or added mod switches and match-scale operations.
- **Remaining levers.** Plaintext coefficient multiplies (12) and alignments (9 mod switches, 9 match-scales) are the only knobs left, and CHEHAB's own normalization seems to erase differences in how I write them.

The kernel to use is the attempt 1 / attempt 3 kernel below.

```cpp
// Per stage (depth 3):
//   t2 = t^2, t4 = t2^2
//   A  = c5*t + (c7*t)*t^2      (= c5 t + c7 t^3)
//   B  = (c3*t)*t^2 + c1*t      (= c3 t^3 + c1 t)
//   p  = t4*A + B               (= c1 t + c3 t^3 + c5 t^5 + c7 t^7)
// 5 ciphertext multiplies per stage at depth 3; final y = x*(t3 + half).

Ciphertext odd_stage(const Ciphertext &t, int s)
{
  Plaintext c1("s" + to_string(s) + "c1");
  Plaintext c3("s" + to_string(s) + "c3");
  Plaintext c5("s" + to_string(s) + "c5");
  Plaintext c7("s" + to_string(s) + "c7");

  Ciphertext t2 = t * t;
  Ciphertext t4 = t2 * t2;
  Ciphertext A = t * c5 + (t * c7) * t2;
  Ciphertext B = (t * c3) * t2 + t * c1;
  return t4 * A + B;
}

void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x;
  for (int stage = 1; stage <= 3; ++stage)
    t = odd_stage(t, stage);
  Plaintext half("half");
  Ciphertext h = t + half;
  Ciphertext y = x * h;
  y.set_output("y");
}
```