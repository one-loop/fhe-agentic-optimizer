The best kernel so far is attempt 1 at 63.53 ms, and attempt 4 is the same kernel at 63.55 ms, within noise. The structure already uses the minimum of two ciphertext-plaintext multiplies and two rotations, and the remaining overhead is one scale alignment. The variants I tried either added a multiply (attempt 3, 65.58 ms) or left the same counts (attempt 2, 63.59 ms), so I'm returning the attempt-1 kernel unchanged.

```cpp
// Reuse t = second*drop; second*keep = second - t, so only 2 pt-mults and 2 rotations are needed.

void fhe()
{
  Ciphertext first("first");
  Ciphertext second("second");
  Plaintext keep_front("keep_front");
  Plaintext drop_front("drop_front");
  const int num_dense = static_cast<int>(slots() / 4);
  Ciphertext t = second * drop_front;
  Ciphertext a = first * drop_front;
  Ciphertext u = a + second - t;
  Ciphertext out0 = u << num_dense;
  Ciphertext out1 = t << num_dense;
  out0.set_output("output_0");
  out1.set_output("output_1");
}
```