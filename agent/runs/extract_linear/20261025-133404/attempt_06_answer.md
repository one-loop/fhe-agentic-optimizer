I have no new circuit to propose. The baseline's single plaintext multiply plus single rotation is the minimum I can find. Every structurally different variant I can build adds operations, and attempt 4 showed that extra rotations make it much slower.

I'm keeping the baseline as the best kernel. Spending more attempts on rewrites that CHEHAB normalizes back to the same circuit would not change the result.

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