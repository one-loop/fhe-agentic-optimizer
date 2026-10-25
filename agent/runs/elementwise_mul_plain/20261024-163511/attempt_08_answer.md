I'm returning the baseline kernel for the last attempt. After seven attempts I have found no circuit for `y = x0 * p` cheaper than one ct-pt multiply plus its rescale. The baseline's 12.92 ms remains the best result, and this resubmission cannot beat it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```