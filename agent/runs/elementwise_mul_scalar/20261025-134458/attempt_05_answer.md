I have no cheaper circuit left to try. The baseline, one scalar ct-pt multiply plus one rescale, appears to be the minimum for this function, and the only different circuit I tried (attempt 2) was slower. Resubmitting it unchanged would again be an identical circuit and cannot win, so I'm not spending another attempt on it. The baseline stays the best result at 4.60 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```