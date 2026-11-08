The baseline is already the cheapest possible circuit for this task, and I have no cheaper structure to try. One plaintext add is the minimum needed to combine `x0` and `p`. Four attempts have now all produced that same circuit, so the baseline remains the best kernel. I'm giving the same kernel again rather than a variant that would only be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```