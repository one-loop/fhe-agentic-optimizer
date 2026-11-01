I still have no cheaper circuit. The baseline is one scalar ct-pt multiply plus one rescale, which is the minimum for multiplying by an unknown plaintext `s`. The only structurally different circuit I tried (attempt 2) was slower at 9.64 ms, so I'm not spending the last attempt on another guess I expect to lose. The baseline remains the best result at 4.60 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```