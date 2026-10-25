No cheaper circuit exists, so I'm returning the baseline kernel. It is the minimal circuit: one ciphertext-ciphertext multiply, with the relinearization and rescale CHEHAB inserts itself.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```