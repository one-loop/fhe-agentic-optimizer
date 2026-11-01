I'm keeping the final kernel unchanged. Attempt 6 measured 29.18 ms, within noise of the best (29.11 ms), and it is the same operation sequence: one ciphertext-plaintext multiply and one rotation. That sequence is the minimum for this function, so no reformulation I can see would be faster.

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