I'm keeping the attempt-1 kernel as the final answer. Its measured time, 128.10 ms, is the best of the four attempts (baseline 177.95 ms). Attempts 2 and 3 were within noise or slightly slower (131.5 ms and 132.5 ms), and I don't see a structural change that would beat it by a meaningful margin.

The remaining cost is two rotations and two plaintext multiplies, plus one level alignment. Attempt 3 removed the alignment but added a third multiply, which netted out to no gain. The kernel below is the attempt-1 version.

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