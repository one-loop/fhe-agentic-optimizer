The baseline is already near the minimum for this function. It uses one plaintext multiply with its rescale, and one plaintext add, with no rotations, ciphertext multiplies or extra alignments. I could not find an equivalent circuit with fewer operations:

- **Folding B into A:** `(x + B/A) * A` needs plaintext-plaintext division, which the DSL does not allow.
- **Dropping the rescale:** the multiply by `A` needs it.
- **Dropping the plaintext add:** `B` has to enter through some plaintext operation.

Below is the same computation with the add written first. CHEHAB will probably normalize it to the baseline circuit, so it may be rejected as not distinct.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```