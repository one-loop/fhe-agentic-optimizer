I don't have a cheaper circuit to propose, and I'd rather say so than dress up another re-spelling.

`y = x*A + B` with inputs `x`, `A`, `B` needs one ct-pt multiply (which brings its rescale) and one plaintext add. Each way I can see to remove one of them is blocked:

- **Folding B into x before the multiply:** `(x + B/A) * A` needs plaintext-plaintext division, which is not allowed.
- **Using constants such as `x + 1`:** CKKS rejects numeric constants.
- **Cancelling terms with `m - m`:** CHEHAB folds it into a constant zero, which is what failed in attempt 2.
- **Rewriting through extra ciphertext operations:** this only adds multiplications, rotations or alignments, so it is slower.

Attempts 1, 3 and 4 all normalized to the baseline circuit, so none of them could win. The baseline circuit is probably already minimal, and the 17.95 ms is mostly one multiply, one rescale and the plaintext encoding. Below is the baseline form again, which is the best correct kernel I have.

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