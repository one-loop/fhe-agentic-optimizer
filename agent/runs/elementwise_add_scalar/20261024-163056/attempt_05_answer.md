I don't think a cheaper circuit exists. Attempt 4 confirmed that extra negations only add work: 3 adds instead of 1, and 1.05 ms against 0.43 ms. The output depends on both `x0` and `s`, so at least one operation is needed, and the baseline already uses exactly one plaintext add. Any other form either compiles to the same circuit, as attempts 1 to 3 did, or adds operations.

I'm submitting the single-add kernel again, written as a plain declaration and a single-expression add. It will most likely be reported as identical to the baseline, and I don't expect it to beat the best.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```