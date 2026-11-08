The baseline is already minimal for this function, so I am returning it unchanged.

`y = x·A + B` takes exactly one ciphertext×plaintext multiply, its required rescale, and one plaintext add. It uses one level, with no rotations, ciphertext multiplications or scale alignments.

There is no cheaper form under the CKKS rules:
- **Moving the multiply** to get `(x + B/A)·A` would need `B/A`. That is plaintext-plaintext arithmetic, which is not allowed.
- **Dropping the multiply** is impossible, because `A` varies per slot and only reaches the ciphertext through a multiply.
- **Moving the add** is pointless. Adding `B` after the rescale already encodes it at the lower, cheaper level.

Because this kernel's circuit matches the baseline, it cannot be judged a new best. Any different circuit would only add operations or levels and would run slower.

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