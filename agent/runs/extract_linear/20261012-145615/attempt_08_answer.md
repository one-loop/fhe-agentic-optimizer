Attempt 7 measured 29.10 ms, a new best by 0.01 ms, which is within noise. The kernel is unchanged: one ciphertext-plaintext multiply and one rotation, the minimum for this function. I'm not using the last attempt, since no reformulation I can find would change the operation count.

```cpp
// One plaintext multiply (mask projection), then one rotation applied after
// the rescale, so the rotation runs at the lower level.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext prod = input * mask;
  Ciphertext output = prod << offset;
  output.set_output("output");
}
```