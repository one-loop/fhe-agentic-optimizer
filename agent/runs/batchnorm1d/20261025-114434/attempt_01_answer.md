The baseline does one plaintext multiply with a rescale, followed by a plaintext add (encoding B costs ~2.5 ms). That is already close to the minimum for y = x*A + B. The only real variation is how B is added.

My idea is to avoid the rescale on the multiply by reordering the arithmetic. Any form that multiplies x by a plaintext still needs a rescale, and A is a non-constant vector, so that multiply can't be removed.

Another option is to fold B into the multiply by writing y = (x + B/A) * A. That needs a plaintext division, which is not allowed. It would also add a plaintext before the multiply, which costs the same encoding and puts the add at a higher level.

That leaves the circuit structurally the same as the baseline. I'll resubmit essentially the same kernel, written with `+=` on the product. CHEHAB will probably normalize it to the same circuit, so it is unlikely to win, but it is a valid kernel.

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