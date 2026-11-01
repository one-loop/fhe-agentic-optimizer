I'm submitting the same kernel again. This run measured 0.26 ms, which matches the baseline and the other repeats of this single addition (0.25 to 0.27 ms).

The kernel performs one ciphertext-plaintext add. That is the minimum work for x0 + s under these CKKS rules, so I don't expect a further change to help.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```