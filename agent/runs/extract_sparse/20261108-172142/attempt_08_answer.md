Attempt 2 remains my best kernel (10.99 ms against the 14.47 ms baseline). I haven't found a circuit that beats it, so I'm submitting it again for the last attempt. Attempts 3–7 all compiled to the same circuit, so none of them counts as a new best.

It uses two identities:
- The rotation is linear, so `output_0` needs only one rotation: `first*drop + second*keep` is rotated once.
- That sum equals `first + (second - first)*keep`, which needs only one plaintext multiplication.

The result is 2 plaintext multiplications and 2 rotations, against 3 and 3 in the baseline, plus one level alignment from adding `first` to the product.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext sum0 = first + ((second - first) * keep_front);
  Ciphertext out0 = sum0 << num_dense;
  Ciphertext out1 = (second * drop_front) << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```