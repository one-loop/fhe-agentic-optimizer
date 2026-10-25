I'm submitting the attempt-1 kernel again as the final answer. Attempt 4 is the same code and measured 129.0 ms, which is within run-to-run noise of the 128.1 ms best. Across the four variants the remaining differences are a few milliseconds, and each alternative I tried either traded a level alignment for an extra multiply or added a rotation, so none of them is a clear improvement.

The kernel uses two plaintext multiplies and two rotations. Rotation is linear, so the two outputs are rotated once each rather than the three rotations in the baseline. The `second*keep` term is rewritten as `second - second*drop`, which lets the `second*drop` product be shared by both outputs.

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