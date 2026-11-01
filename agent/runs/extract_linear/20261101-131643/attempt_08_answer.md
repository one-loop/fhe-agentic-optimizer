This is the last attempt, and I have no cheaper circuit to try. The baseline `(input * mask) << slots()/4` already uses only one ct-pt multiply with its rescale and one rotation, and every correct alternative costs at least that much.

Across the attempts, only attempt 2 tried a different structure (rotating the mask), and CHEHAB rejected it. The other attempts all repeated the baseline circuit; attempt 1 did so by mistake, and the later ones on purpose. The result is no improvement over the 29.18 ms baseline.

Final kernel:

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