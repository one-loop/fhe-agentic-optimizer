I'm keeping the baseline as the final kernel. This attempt measured 59.61 ms against the baseline's 59.11 ms, which is noise, and it has the same operations (one ct-pt multiply, one rotation, one rescale). Every variant so far has come out within a few milliseconds of the baseline. The only alternative ordering, rotating before the multiply, was rejected by the backend because it requires a plaintext rotation. I don't have a formulation with fewer rotations or multiplies, so further attempts are unlikely to help.

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