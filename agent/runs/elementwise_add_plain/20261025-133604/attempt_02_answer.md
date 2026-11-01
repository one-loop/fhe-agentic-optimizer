The baseline is a single plaintext add, which is already the minimum for this function. Any cheaper circuit would have to drop the add or the encoding of `p`, and that isn't possible. I'm trying one variant, `-((-x0) - p)`, to see whether CHEHAB's rewrites produce a different, cheaper circuit. I expect it to be normalized back to the baseline or to add negations.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext n = -x0;
  Ciphertext m = n - p;
  Ciphertext y = -m;
  y.set_output("y");
}
```