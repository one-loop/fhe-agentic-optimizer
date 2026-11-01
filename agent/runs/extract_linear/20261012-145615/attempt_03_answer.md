I'm stopping here rather than spending more attempts. The kernel has one ciphertext-plaintext multiply and one rotation, and the timings sit within noise of the baseline (29.11–29.13 ms). I don't see a formulation that removes the rotation or lowers its cost without adding work.

The rotation is required because output slot i must hold input slot i+k. Rotating the input first would place it at full level, which costs more than rotating after the rescale. Rotating the mask instead would need plaintext shift support that the DSL does not document, and it would still add a full-level rotation.

Best kernel (attempt 1, 29.11 ms):

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