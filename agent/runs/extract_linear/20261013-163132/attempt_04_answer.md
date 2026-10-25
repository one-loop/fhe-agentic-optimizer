I'm stopping the search here and returning the baseline kernel as the final answer. Every variant I tried has the same operation count (one ct-pt multiply, one rotation, one rescale) and timings land between 59 and 63 ms, so the differences are noise. The only structural alternative, rotating before multiplying, was rejected by the backend. I don't see a cheaper formulation that avoids the rotation, since output slot i must read input slot i+offset.

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