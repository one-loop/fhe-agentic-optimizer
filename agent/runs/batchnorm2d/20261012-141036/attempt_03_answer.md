I'm stopping the search here. The kernel is already at the minimum work for this function, and the last three runs (9.84, 9.79, 9.87 ms) differ only by noise. Any further attempt would be the same kernel with the same measured time.

The required work is one ciphertext-plaintext multiply, since `A` varies per slot, and one plaintext add, since `B` is a per-slot shift. Removing either would need an operation the rules forbid, such as plaintext-plaintext arithmetic to precompute `B/A`, or a numeric constant.

Best kernel (attempt 1, 9.79 ms, max error 2.59e-08):

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// Minimal evaluation for this function; no further reduction in operations is possible.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```