I'm resubmitting the same kernel. This attempt measured 0.26 ms, in line with the baseline and the other repeats of the identical addition (0.25 to 0.27 ms), so there is no real change to report.

The computation is one ciphertext-plaintext add, which is the minimum the CKKS rules allow for x0 + s. Nothing I've considered would remove that operation or make it cheaper.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```