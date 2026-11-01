I think attempt 1 is already the minimal circuit for this task, and I don't have a variant I expect to be faster.

**Why attempt 1 looks minimal**
- **Rotations (2):** each output is a full shifted ciphertext, and the two outputs come from different data.
- **Plaintext multiplies (2):** `output_1` needs `second` masked. `output_0` needs `first` masked, and that cannot share a multiply with `second` without extra rotations. Plaintext masks cannot be rotated and constants are not allowed.
- **Alignment (1 `match_scale`):** this is the only extra cost. After a rescale the scale is no longer exactly the original, so the level-0 `second` needs a `match_scale` rather than a cheap mod switch.

**What I ruled out**
- **Three multiplies instead of the alignment:** this was attempt 2. It came out slightly slower: 65.67 ms against 64.28 ms in single timings.
- **Rotating each term separately:** this needs 3 rotations, about 20 ms more.
- **Rotating before masking:** this needs a rotated mask, which isn't available.

The kernel below is attempt 1 again, the best design found. It will register as the same circuit and won't count as a new improvement.

```cpp
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t1 = second * drop_front;
  Ciphertext t0 = first * drop_front;
  Ciphertext out0 = ((t0 - t1) + second) << num_dense;
  Ciphertext out1 = t1 << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```