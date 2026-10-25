The operation counts match the baseline exactly (one ct-pt multiply, one rescale, one add), so the 18.16 ms vs 19.28 ms gap looks like run-to-run noise rather than a real improvement. I don't see a legitimate way to cut work further: the multiply by `A` and the add of `B` are both required, and nothing else in the computation can be eliminated. I'm keeping the same kernel.

```cpp
// y = x * A + B. Minimal form: one ct-pt multiply (one level) plus one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```