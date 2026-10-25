I'm keeping the same kernel. Attempts 1 and 2 both come out at about 18.2 ms with identical operation counts, so the spread against the 19.28 ms baseline is timing noise, not a change in the program.

The kernel is already at the minimum for this function. The multiply by `A` is required and the add of `B` is required, and each costs one operation. Nothing can be removed without changing the result, and there are no rotations or ciphertext-ciphertext multiplies to eliminate.

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