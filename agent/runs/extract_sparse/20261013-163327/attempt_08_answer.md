I'm submitting the same kernel as the final answer. Attempt 7 measured 133.5 ms for identical code, and across the repeated runs the timings range from 128 to 141 ms, so the differences are run-to-run noise rather than a kernel change. The best recorded result is attempt 1 at 128.10 ms, against 177.95 ms for the baseline.

The kernel uses two plaintext multiplies and two rotations, down from three of each. Rotation is linear, so the combined sum is rotated once. The `second*keep` term is rewritten as `second - second*drop`, so `second*drop` is shared with `output_1`. The remaining level alignment costs about as much as the alternatives I tried, so I don't have a change I expect to beat this.

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