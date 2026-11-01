The idea is to replace the general ciphertext-ciphertext multiply `x * x` with `square(x)`. SEAL's dedicated squaring routine needs fewer polynomial products than a general multiply: 3 instead of 4 for size-2 ciphertexts. The result, relinearization and rescale are unchanged, and the circuit differs from the baseline (square=1, ct_ct_mul=0). It should come out somewhat faster, though I haven't timed it yet.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```