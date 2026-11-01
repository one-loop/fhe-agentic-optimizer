I don't think this kernel can be made meaningfully faster. One squaring is the least possible work for y = x·x: one multiply, one relinearization and one rescale. The usual savings don't apply here:
- **Rotations and plaintext multiplies:** there are none to remove.
- **Depth:** it is already 1.
- **Computing at a deeper level:** I would need to mod-switch `x` down before squaring. CHEHAB only inserts mod switches when it aligns two operands at different levels, and that needs a second operand. The only extra operand available here is a plaintext constant, which the task doesn't provide (CKKS rule 1). Any extra term would also cost more than the ~1 ms it might save.
- **Algebraic rewrites:** forms like `square(x + x)` need a constant such as 1/4 to scale back, which isn't allowed. Forms like `(x+x)*x - x*x` add multiplies.

`square(x)` was faster in all 5 alternating batches, but only by about 1.5% (31.00 vs 31.47 ms), short of the 3% bar. Resubmitting it can't win, since it has already been tried. It is still the best circuit I can see, so I'm giving it as the final kernel rather than spending attempts on circuits that are certainly slower.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```