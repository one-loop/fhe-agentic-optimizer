The baseline is still the best circuit I can find. Every attempt has compiled to the same one: a single ciphertext-plaintext multiplication with one rescale, at depth 1 and one level. Resubmitting it again would not change the result, and no alternative removes the multiplication or the rescale. I'm submitting it once more for the record.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```