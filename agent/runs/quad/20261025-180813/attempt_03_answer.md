The multiplication is the floor here: x² needs one ciphertext multiplication, and any rewrite adds work. The negation variant was within noise of the baseline (0.99x, faster in 1 of 5 batches).

I'll try one more variant: `(x + x) * x` is 2x², so I can't use it directly. Instead I'll go back to the plain product with no extra operations. It is the same circuit as the baseline and can't win, but I have no cheaper form to offer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```