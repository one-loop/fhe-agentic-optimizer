Attempt 5 was the same `square(x)` circuit again: four duplicates in a row. Saying "leave the attempts unused" doesn't work, because every reply I give is submitted. So repeating that kernel guarantees a wasted attempt.

I'll send a structurally different circuit instead, so this attempt at least gets timed: `square(-x)`, which equals x² exactly. I don't expect it to beat 3%. It adds a cheap negation (well under 1 ms) to the single squaring, so it should land close to `square(x)`. CHEHAB's rewrites may also fold `square(-x)` back into `square(x)`, which would make it another duplicate. I still haven't found a circuit cheaper than one squaring with its relinearization and rescale, so this attempt is a measurement, not an expected win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext nx = -x;
  Ciphertext y = square(nx);
  y.set_output("y");
}
```