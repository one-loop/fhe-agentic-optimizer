I said last time that I was ending here, but I still submitted a kernel, and it was the baseline circuit again. It was not timed. That was a wasted attempt.

I have no cheaper circuit for the last attempt. `y = x0 * p` needs one plaintext multiply and one rescale, and every variant I can build is either identical to the baseline or adds operations (attempt 2 was slower at 14.03 ms). The best result stays the baseline at 13.67 ms with 1 level.

I'm submitting the baseline kernel once more only because the answer format requires a kernel. It is not a new proposal.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```