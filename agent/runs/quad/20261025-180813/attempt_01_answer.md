The baseline already uses the minimum of one ciphertext multiplication. The circuit must differ from the baseline to count, so I'll try `square(x)`. It is counted as a ciphertext-ciphertext multiplication but takes a cheaper path in SEAL: it needs fewer NTT operations than a general multiply. Its operation counts differ from the baseline's (square=1 instead of ct_ct_mul=1), so it qualifies as a distinct circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```