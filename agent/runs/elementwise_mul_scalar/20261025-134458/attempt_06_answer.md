I can't find a cheaper circuit, and I should correct my last message. I said I wouldn't resubmit the baseline, but the code block I included was the baseline anyway, which wasted attempt 5.

The baseline is one scalar ct-pt multiply plus one rescale. Getting below that would mean skipping the multiply or the rescale, and with `s` available only as a plaintext input that isn't possible. The only different circuit I tried, in attempt 2, was slower at 9.64 ms.

The baseline remains the best result at 4.60 ms. I'm leaving the remaining attempts unused rather than submitting circuits I expect to be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```