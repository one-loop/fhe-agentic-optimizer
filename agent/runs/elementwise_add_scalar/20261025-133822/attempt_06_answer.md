The baseline is still one ciphertext-plaintext add, and I see no circuit with fewer operations that computes x0 + s. Any change in structure would add a multiplication, rotation, rescale or alignment, which costs more than the 0.27 ms add. Rewrites that CHEHAB normalizes back to the same circuit are judged identical, as the last five attempts showed.

I am submitting the baseline kernel unchanged. It cannot be recorded as an improvement, but it is correct, and I would rather not submit a slower variant just to look different.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```