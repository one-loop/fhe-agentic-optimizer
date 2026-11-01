I'm submitting the same kernel once more. This run measured 0.26 ms, the same as the baseline, and the spread across repeats (0.25 to 0.27 ms) is noise for one addition.

The computation is one ciphertext-plaintext add, the minimum the rules allow. Nothing else I can construct would remove or cheapen that operation.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```