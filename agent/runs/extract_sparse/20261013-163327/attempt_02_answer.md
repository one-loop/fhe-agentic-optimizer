The remaining `match_scale` comes from adding the level-0 `second` to the level-1 `F - D`. Every way I can see to remove it costs at least as much: a rotation of `second` separately is about 20 ms, and multiplying `second` by an all-ones plaintext is about 5 ms, the same as the alignment. The `(first - second)*drop + second` form has the same mismatch. So I'm keeping the attempt-1 structure, which is the best measured so far at 128 ms.

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