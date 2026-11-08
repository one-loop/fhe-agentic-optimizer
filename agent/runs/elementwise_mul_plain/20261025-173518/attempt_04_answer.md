Resubmitting the baseline again cannot win, so I need a different circuit that is cheaper. The multiply is required, since the output depends on p, and I can't remove the rescale either. The remaining question is whether some variant of the same multiply costs less.

One candidate is to get the multiply done on a ciphertext that has already been mod-switched to a lower level. A mod switch costs about 1 ms, and the multiply and rescale at a deeper level may be cheaper, as the cost notes say operations get cheaper with fewer levels remaining. Mod switches only appear when CHEHAB aligns operands, though, and I can't emit one directly.

A way to trigger one is to add a term with deeper history. For example, I could compute x0*p, then add (x0*p)*p' terms, but that needs a second plaintext input, which the task doesn't have. Squaring p isn't allowed either, because plaintext-plaintext arithmetic is forbidden.

Any such trick adds more cost than it saves. So I'm trying a different approach: write y as a sum of two half-products, y = x0*p + (x0 - x0)*p. This is likely normalized away to the baseline, but it's cheap to test. It differs from my earlier variant in that the second term is zero.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext z = x0 - x0;
  Ciphertext y = x0 * p + z * p;
  y.set_output("y");
}
```