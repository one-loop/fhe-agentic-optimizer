The baseline's single ct-pt multiply plus rescale looks like the floor, and CHEHAB normalizes away x0 - x0 style rewrites (attempt 4 collapsed back to the baseline). Every circuit that differs from it has so far added operations.

One remaining idea is to change the multiply itself. Rotating x0 and p's roles doesn't help, since rotations cost about 20 ms each. A rotate by 0 is probably removed. A ct-ct multiply is not an option, because p is a plaintext input.

I'll try a decomposition that is not algebraically cancelled: y = (x0 * p) + (x0 * p) - (x0 * p). Common-subexpression elimination will probably merge the three copies, but I haven't checked, so I'm testing it. It is the same family as before and probably collapses to the baseline, but it uses a different expression shape than what I tried.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext a = x0 * p;
  Ciphertext y = a + a - a;
  y.set_output("y");
}
```