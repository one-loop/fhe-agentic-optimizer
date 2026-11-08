No different circuit can be faster here, so I am returning the baseline again. This means the attempt will not count as a new best.

Every operation in the baseline is required:
- **The multiply:** `A` varies per slot and can only reach the ciphertext through one ciphertext×plaintext multiply.
- **The rescale:** CHEHAB inserts it automatically after the multiply, and I cannot remove it from the DSL.
- **The add:** `B` can only enter through one plaintext add, and it is already applied at the lower, cheaper level.

None of the operation-saving rewrites are possible:
- **Adding first:** `(x + B/A)·A` would need plaintext-plaintext division, which is not allowed.
- **Splitting `A` or `B`:** `x·A1·A2` or `+ B1 + B2` would need other plaintext inputs, which this task does not have. It would also add multiplies, levels or encodings.
- **Rotations:** there is no cross-slot work, so rotations can only add cost.

Any circuit that differs from the baseline has to add operations or levels. It would be slower by more than the 3% margin you require.

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