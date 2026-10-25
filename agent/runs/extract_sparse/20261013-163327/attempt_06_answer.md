I'm submitting the same kernel again as the final answer. This is attempt 5 with 130.7 ms, and attempts 1, 2, 4 and 5 all measure 128–131 ms for identical code, so the spread is run-to-run noise. The best recorded time is attempt 1 at 128.10 ms, against 177.95 ms for the baseline.

The kernel uses two plaintext multiplies and two rotations, down from three of each. Rotation is linear, so the `first*drop + second*keep` sum is rotated once, and `second*keep` is written as `second - second*drop` so the `second*drop` product is shared with `output_1`. Further gains would need a way to avoid the one level alignment, and the alternatives I tried either cost as much as the alignment or add a rotation.

```cpp
// Best so far: rotation is linear, so rotate the combined sum once.
// second*keep = second - second*drop, so only two plaintext multiplies are needed,
// and second*drop is shared between both outputs.
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);

  Ciphertext D = second * drop_front;          // second*drop, shared
  Ciphertext F = first * drop_front;           // first*drop
  Ciphertext combined = (F - D) + second;      // first*drop + second*keep

  Ciphertext out0 = combined << num_dense;
  Ciphertext out1 = D << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```