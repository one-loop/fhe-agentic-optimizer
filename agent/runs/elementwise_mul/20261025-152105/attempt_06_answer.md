I have no cheaper valid circuit, so I'm resubmitting the baseline once more. The only exact alternatives either add multiplications or need a constant CKKS doesn't allow. Resubmitting can't produce a new best, and I'd rather say so than present a variant that is slower or wrong.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```