I haven't found a cheaper circuit. This last attempt is the baseline again, because the mask multiply and the rotation are both required and the circuit already has the minimum number of each. Any correct rewrite I tried either reduced to this same circuit or was rejected. I expect it to be rejected as identical once more, so the baseline is the best result I can report.

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