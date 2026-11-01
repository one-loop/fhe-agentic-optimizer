The baseline is still the minimal circuit, so I'm returning it unchanged.

`x0 * x1` needs one ciphertext-ciphertext multiplication, with its automatic relinearization and rescale, at one level. That is the entire baseline, and any other structure adds cost. The final result is the baseline at 31.58 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```