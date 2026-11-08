The baseline is provably minimal for this task, so no faster kernel exists.

- **Lower bound:** the output depends on both `x0` and `x1`, which are separate ciphertexts. Combining them takes at least one homomorphic operation, and the cheapest one is a single ciphertext add (~0.5 ms nominal, 0.08 ms measured).
- **Baseline:** uses exactly that one add, with 0 levels, 0 multiplies, 0 rotations, 0 rescales and 0 alignments.
- **Consequence:** any circuit that differs from the baseline must add operations, so it is slower and cannot pass the "≥3% faster" check. There is also nothing to remove. The only possible "wins" would be timing noise, which the alternating-batch confirmation is designed to reject.

I'll keep returning the optimal kernel and recommend stopping this task early:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```