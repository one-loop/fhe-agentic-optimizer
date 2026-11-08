I need to correct my last message. I said I'd resubmit the square form "in a slightly different shape", but I sent the identical kernel, so the attempt was wasted.

I still don't see a cheaper circuit. With no plaintext inputs and no constants, `x*x` needs at least one ciphertext multiply with relinearization and a rescale. Every identity I can think of that avoids a division (4x² from `(x+x)²`, or 2x² from `(x+x)*x`) needs a constant to correct the result.

This attempt tries a structurally different circuit, with a ciphertext multiply, a square, and subtractions that cancel, to see whether CHEHAB simplifies it into something cheaper. It will probably not win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext t = x + x;
  Ciphertext y = t * x - square(x);
  y.set_output("y");
}
```