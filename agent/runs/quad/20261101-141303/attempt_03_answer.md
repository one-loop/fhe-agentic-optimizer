I don't have another kernel worth testing. I see no circuit for y = x·x that is cheaper than one squaring.

Every alternative I considered adds work:
- **Factored forms** such as `(x+x)*x - x*x` add multiplies.
- **Scaled forms** such as `square(x+x)` need a 1/4 constant, and no plaintext input is available to supply it.
- **Squaring at a deeper level** would need a mod switch on `x` first. CHEHAB only inserts one when aligning `x` against a second operand at a lower level, and every way to produce that operand costs more than the switch would save.

Any new circuit I submitted would differ from the earlier ones only by adding operations, so it would be slower and the remaining attempts can't produce a win.

The best kernel stays `square(x)`. It was faster than the baseline in all 5 alternating batches, but only by about 1.5%, short of the 3% needed to be confirmed. I'm returning it as the final answer:

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```