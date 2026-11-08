The baseline is one plaintext multiply with its rescale, then one plaintext add. The add costs about 2.5 ms for encoding, and the multiply costs about 7 ms.

I can't skip the multiply, since A is a general vector. One option is to fold B into the multiply, but that needs a ciphertext of ones, which constants don't allow.

A second option is to reorder the computation so the encode and add happen at a different level, but the scale is the same either way. The add of B after the rescale already happens at the lower level, which is cheap.

The evaluation is already near minimal at 1 multiply plus 1 add. The only change I can try is writing it as `b + x*a`, which CHEHAB probably normalizes to the same circuit. I'll submit it anyway, since there's little else to try.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext y = m + b;
  y.set_output("y");
}
```