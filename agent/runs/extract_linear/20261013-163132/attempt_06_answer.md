I'm returning the baseline kernel again as the final answer. This run measured 59.50 ms, consistent with the other runs of the same operation sequence (59.11–63.48 ms), so the spread is measurement noise. The kernel already uses the minimum work for this task: one ciphertext-plaintext multiply and one rotation at reduced level after rescale. The only other ordering I tried was rejected by the backend, and I don't have a way to cut either operation.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```