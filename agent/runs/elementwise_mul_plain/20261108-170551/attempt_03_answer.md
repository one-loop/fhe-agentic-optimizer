I still see no cheaper circuit, so I'm not going to burn more attempts on variants that reduce to the same one. The kernel is already at the minimum: one plaintext multiply and one rescale on a single level, with no rotations, ciphertext multiplications or alignments.

Every alternative I can think of adds operations:
- **Square identity:** `((x+p)^2 - (x-p)^2)/4` needs ciphertext multiplications and plaintext-plaintext arithmetic, which the rules forbid.
- **Splitting `p`:** this adds more plaintext multiplies and adds.
- **Extra adds or alignments:** these only add cost.

I'm resubmitting the baseline kernel. It is still the best circuit I know of, even though it will be rejected as identical.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```