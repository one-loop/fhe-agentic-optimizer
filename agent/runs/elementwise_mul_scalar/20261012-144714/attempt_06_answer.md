I'm keeping the same kernel. Attempt 5 measured 4.59 ms, the same as the baseline, and attempt 4's 4.58 ms was within noise. Any change would have to remove the scalar multiply or the rescale, and neither can be removed without changing the computation.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```