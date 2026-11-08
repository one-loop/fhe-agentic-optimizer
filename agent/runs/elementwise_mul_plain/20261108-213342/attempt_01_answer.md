The baseline is already the minimal circuit for this function. Computing `y = x0 * p` with an arbitrary dense plaintext vector `p` needs at least one ciphertext × plaintext multiply. In CKKS that multiply must be followed by a rescale, and CHEHAB inserts the rescale automatically. The baseline is exactly that: one `ct_pt_mul`, one `rescale`, no rotations, no ciphertext multiplies, one level.

Every rewrite that changes the circuit adds work. Examples are splitting `p`, adding rotations, or masking. Each one adds plaintext multiplies, rotations or levels and would be slower. No alternative circuit can be both correct and faster.

I'm resubmitting the minimal kernel. Its circuit is identical to the baseline's, so it can't be ranked as a new best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```