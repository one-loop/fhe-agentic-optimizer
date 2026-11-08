On this last attempt I'm submitting the baseline again, because no cheaper circuit exists. None of the 8 attempts improved on it.

The output needs one ct-pt multiply by the runtime plaintext `s`. CKKS then requires one rescale, which CHEHAB inserts itself. The baseline contains exactly these two operations at 1 level, so any other structure would add work. The final best is the baseline at 1.66 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```