The baseline has one plaintext multiply with its rescale, plus one plaintext add (which includes the encoding cost). The function is y = x*A + B, so a multiply by A is unavoidable, and the structure is already near-minimal.

One option is to change the circuit so the add of B happens without the encoding path. The two forms are:
- (x*A) + B: a plaintext add at the lower level. B has to be encoded at the post-rescale scale.
- Adding B before the rescale is not expressible in the DSL.

Another option is to fold B into the multiply, but that needs a ciphertext of ones, which we don't have.

The most plausible small change is to reorder the expression as `x*a - (-b)`. That is the same circuit, so it won't count as different. Since the circuit has to differ to count, I'll try computing it via a different but equivalent form: y = (x + B/A) * A. This costs a plaintext division, which is forbidden, so it is not possible.

I'll submit the straightforward variant, which keeps the circuit minimal. It may be judged identical to the baseline, but there is no cheaper correct alternative that I can see.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext y = t + b;
  y.set_output("y");
}
```