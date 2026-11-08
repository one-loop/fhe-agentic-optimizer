I can't find a cheaper circuit for this task. x² needs exactly one ciphertext multiplication with its relinearize and rescale. There are no plaintext inputs to supply constants, and I can't emit a mod switch myself, so any other identity only adds operations. The variants I tried or considered were `x * x`, `square(x)`, and splitting into `(x+x)` terms. The first two give the same circuit up to timing noise, and the split adds multiplications and needs a division by 4 that CKKS can't express without constants.

I'm resubmitting the minimal one-multiply kernel, written with the plain multiplication operator. It is the baseline form, which is the cheapest circuit I know of.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```