The result of attempt 1 shows that square gives only about 1% gain. The function is a single multiplication, so the remaining cost is in the multiply, relinearize and rescale. There is no way to avoid the multiplication, because a nonlinear function needs at least one ct-ct multiply.

One option is to skip relinearization by working on the size-3 ciphertext, but CHEHAB inserts relinearization automatically, so I can't control it. I'll try an algebraically different form that might lower the cost: x*x = ((x+x)*(x+x))/4 doesn't help, because we can't divide by a constant without a plaintext multiply.

Another idea is to split the product, for example (x+x)*x = 2x². That still needs one multiply plus a plaintext correction, so it is no cheaper.

I can't find a cheaper circuit than one multiply, so I'll resubmit the square form with a small variation: computing the square from a rotated-by-zero copy changes nothing either. The best remaining option is the same minimal circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```