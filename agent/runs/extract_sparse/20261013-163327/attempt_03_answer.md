Attempt 2 is the same kernel as attempt 1 and measured within noise (131.5 ms vs 128.1 ms), so it doesn't add information. The one structural alternative left is to avoid the level mismatch entirely: compute `G = second*keep` as its own multiply, so `first*drop` and `second*keep` both sit at level 1 and their sum needs no alignment. That trades the `match_scale` (about 5 ms) for a third plaintext multiply (about 7 ms), so it may be slightly slower. I'm testing it to measure rather than assume.

```cpp
// Experiment: all mask products are single plaintext multiplies at level 1,
// so the sum needs no level alignment. Rotations: 2 (combined and second*drop).
void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);

  Ciphertext F = first * drop_front;     // first*drop
  Ciphertext G = second * keep_front;    // second*keep
  Ciphertext D = second * drop_front;    // second*drop, for output_1

  Ciphertext out0 = (F + G) << num_dense;
  Ciphertext out1 = D << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```