The baseline circuit looks minimal: one ct-pt multiply with its rescale, plus one plaintext add. I can't fold B in before the multiply, since that needs B/A and plaintext arithmetic is not allowed. I also can't drop the rescale. This attempt uses in-place accumulation (`y *= a; y += b`) in case CHEHAB lowers it to a different circuit. It will probably normalize to the same one.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x;
  y *= a;
  y += b;
  y.set_output("y");
}
```