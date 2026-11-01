I'm stopping the search. Six attempts all land between 29.11 and 29.42 ms, the spread is run-to-run noise, and the kernel's cost is set by its operation count. The kernel does one ciphertext-plaintext multiply and one rotation, which is the minimum for this function. The rotation is required because output slot i must hold input slot i+k. Rotating the input first would run at full level, which costs more. Rotating the mask would need plaintext shift support that the DSL does not document, and it would still add a full-level rotation. Spending the remaining attempts would most likely just produce more noise.

Final kernel (attempt 1, 29.11 ms):

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