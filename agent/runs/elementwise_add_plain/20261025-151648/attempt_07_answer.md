Resubmitting the baseline has now failed five times, so this attempt changes the structure. I'm computing the sum as `-((-x0) - p)`. It is mathematically identical but has a different operation graph: a negation, a plaintext subtraction and another negation.

I expect it to be slower, or to be normalized back to the baseline, because the negations add ciphertext work. It does show whether CHEHAB leaves a different circuit in place, and I don't have a cheaper candidate to try instead.

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