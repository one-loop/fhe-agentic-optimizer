Attempt 2 was a mistake on my part: I resubmitted the identical circuit, so it could not be evaluated.

Both forms use one ct-ct multiply, one relinearization and one rescale, and a nonlinear function needs at least one ct-ct multiply, so I can't remove that. What remains is to change the circuit's structure, with the risk that it ends up slower.

Idea: use (a+b)² identities to move work onto cheaper operations. For example, x² = ((x+x)·x)/2 needs a division. Another route is `(x + x) * (x + x)`, which is 4x² and also needs a scale-down by a plaintext multiply. Each of these adds operations, so none is likely cheaper than the plain square.

A cheaper rewrite would have to lower the cost of the multiply itself. The mod-switch variant doesn't do that either, since the multiply already runs at the top level where it is most expensive. Dropping a level first with a cheap mod switch would make the multiply cheaper, but CHEHAB only inserts mod switches for alignment, and I have no way to request one directly.

One way to trigger an alignment is to multiply a ciphertext at a deeper level against a shallower one. With a single input and no plaintext I can't build that without extra multiplies, which cost more than they save.

I'll try the form `x * x` written as a product of sums, `(x + x) * x`, as a structurally different circuit with an add. It computes 2x², which is wrong, so I won't submit it. The honest option is to submit the plain square once more with a different but equivalent structure that CHEHAB might not normalize away: the difference of squares x² = (x+x)²/4 − ... also needs constants, so it isn't usable.

Since every alternative I can think of adds operations, the best I can do is test one cheap variation: `x * x` computed as `square(x)` followed by an add of a zero-valued term, which has no effect. I don't expect that to help, so I'm submitting the original `x * x` form once more, which is the best-known circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```