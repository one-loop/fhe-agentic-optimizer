Attempt 2 shows that a third plaintext multiply costs about as much as the `match_scale` it removed. So attempt 1's structure is probably close to minimal:
- **Rotations:** two outputs, each a full shifted ciphertext.
- **Multiplies:** two masking multiplies, since plaintext masks cannot be rotated and constants are not allowed.
- **Alignment:** one alignment for the level-0 `second`.

This variant subtracts at level 0 before masking: `output_0 = ((first - second) * drop_front + second) << num_dense`, with `output_1 = (second * drop_front) << num_dense`. CHEHAB may give it different operation placement or counts from attempt 1, but it may also normalize to the same circuit. Its operation counts look identical to attempt 1's, so the judge may reject it before timing. If so, attempt 1 should be treated as the best design found.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t1 = second * drop_front;
  Ciphertext out0 = ((first - second) * drop_front + second) << num_dense;
  Ciphertext out1 = t1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```