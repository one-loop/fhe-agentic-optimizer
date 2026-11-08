The circuit is already one add. Cheaper alternatives are hard to find, since any other form needs at least as many operations. The cost of that add is mostly encoding the scalar plaintext (~2.5 ms for a vector, less for a scalar), and I can't avoid that while still using `s`.

One thing worth trying is to compute the sum with a different operation structure, for example `x0 - (-s)`. Negating a plaintext is plaintext arithmetic, which is not allowed, so that is out. Negating the ciphertext instead (`-(-x0 + ...)`) adds operations.

The only distinct circuit that stays correct and might not cost more is a subtraction: `y = -((-x0) - s)`. This has more operations (two negations plus a subtraction), so it will almost certainly be slower. I'm not submitting it.

I'm submitting the minimal form again. I can't produce a legitimately cheaper circuit, and padding the kernel with extra operations would only make it slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```