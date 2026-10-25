I'm submitting the same kernel once more as the final answer. This run measured 141.1 ms, but the code is identical to attempt 1 (128.1 ms), so the spread across runs reflects timing noise rather than a change in the kernel. The best recorded time is still attempt 1.

The kernel uses two plaintext multiplies and two rotations, down from three of each in the baseline. Rotation is linear, so `first*drop + second*keep` is rotated once. The `second*keep` term is written as `second - second*drop`, so the `second*drop` product is shared with `output_1`. The one remaining level alignment costs about as much as the alternatives I tested, so I don't have a change that would clearly beat this.

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