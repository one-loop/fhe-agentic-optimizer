No cheaper circuit exists for this task. One plaintext multiply, with the rescale CHEHAB adds to it, is the least work that can compute `x0 * p`:

- **Data dependence:** `p` is a dense plaintext vector with arbitrary values, so the output needs a slot-wise multiply of `x0` by `p`. There is no way to get it from additions or rotations.
- **CKKS rules:** constants are not allowed, and neither is plaintext-plaintext arithmetic. So `p` cannot be pre-transformed or split in a way that avoids the multiply.
- **Alternatives:** every other circuit adds work. Splitting `p` into parts adds more `ct_pt_mul` and adds. Rotation or masking tricks add rotations at about 20 ms each. Squaring tricks add ct-ct multiplies and levels. Each of these would be correct but slower than the 2.89 ms baseline, so none can win.

I recommend ending the search here; the remaining attempts can only produce slower circuits or repeat the baseline. For completeness, this is the optimal kernel, and its circuit is identical to the baseline's:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```