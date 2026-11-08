The baseline is already a single ciphertext addition, which is the minimum for this function. The output depends on both inputs, so at least one operation is required. I can't remove any operation, and any rewrite would either keep the same circuit or add work. I'm therefore resubmitting the minimal kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```