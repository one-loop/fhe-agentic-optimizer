I have no real structural improvement. Without a plaintext input, every correct circuit needs one ct-ct multiply with its relinearize and rescale, and any rewrite I can derive adds operations. My last two attempts repeated earlier circuits, which was a waste.

This attempt tries something different: `(x + x) * x - x * x`, which equals x² and costs two multiplies. That can only be slower. A better use of an attempt is a form that CHEHAB might lower differently without adding multiplies: `square(x)` computed on `x` after an exact add and subtract, `(x + x) - x`. This equals x, so the result is still x², and it may change the scheduling.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = (x + x) - x;
  Ciphertext y = square(t);
  y.set_output("y");
}
```