The baseline is a single ciphertext-plaintext add, and I see no cheaper circuit for x0 + s. Every alternative I can think of adds operations, such as a multiplication, rotation or rescale, and would be slower.

I'm resubmitting the same kernel. It will be judged identical to the baseline again, so it can't count as an improvement. I would rather say that than present a slower variant as progress.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```