Attempt 3 measured 0.25 ms, a 0.01 ms difference from the baseline. That is within run-to-run noise for a single addition, so it is not a real improvement. The kernel is the same single ciphertext-plaintext add in all four attempts, and nothing in the CKKS rules permits a cheaper formulation.

I'm keeping the same kernel as the final answer. It is the best measured version, and it is correct to 2.5e-08 against the 1e-06 tolerance.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```