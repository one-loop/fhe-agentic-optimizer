The only remaining cost is the plaintext add, which includes encoding the scalar. A different circuit has to cut that cost, and I see one option.

The plaintext `s` is a scalar, so it may be encoded as a single value broadcast to all slots. Adding it as `s + x0` would be normalized to the same circuit as the baseline. Any attempt to avoid the encoding would need constants or plaintext arithmetic, which the rules forbid.

One structurally different variant is `(x0 + s) + x0 - x0`, but that adds operations and is strictly slower. I'm trying a variant that computes `x0 - (x0 - s)`. It is mathematically equal to `x0 + s` but uses two plaintext/ciphertext subtractions, so it will likely be slower. It is still a distinct circuit, so I'm submitting it to see how the measurement behaves.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext t = x0 - s;
  Ciphertext y = x0 - t;
  y.set_output("y");
}
```