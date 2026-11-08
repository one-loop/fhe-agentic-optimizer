I recommend ending the search here: the baseline is the cheapest circuit for this task, so the remaining attempts cannot produce a faster correct kernel.

The baseline uses one ciphertext×plaintext multiply, one rescale (inserted automatically by CHEHAB) and one plaintext add. It consumes one level and needs no rotations, ciphertext multiplications or scale alignments. Each operation is required, and no rewrite removes any of them:
- **No add-first form:** `(x + B/A)·A` would need plaintext-plaintext division, which CKKS forbids here.
- **No way to drop the multiply:** `A` differs per slot, so it can only reach the ciphertext through a multiply. That multiply always brings a rescale.
- **No cheaper add:** `B` enters through one add, and that add already happens at the lower, cheaper level.

Every structurally different equivalent adds multiplies, rescales, alignments or rotations. Each of those costs about 1–20 ms, against a total of 3.73 ms, so none can be 3% faster.

Below is the optimal kernel once more. I will not submit a deliberately worse circuit just to make it different.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```