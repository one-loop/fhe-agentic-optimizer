I'm returning the baseline kernel once more. I still can't find a circuit for `y = x0 * p` cheaper than one ct-pt multiply plus its rescale, so the baseline's 12.92 ms stands as the best result. This resubmission can't be judged faster than itself.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```