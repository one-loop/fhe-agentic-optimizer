I still don't see a cheaper circuit: one ct-ct multiply is the minimum for x², and any rewrite only adds work. Here is a structurally different but equivalent form, which I expect to be slower and which is mainly a probe of how CHEHAB treats it.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext a = x + x;
  Ciphertext y = a * x - x * x;
  y.set_output("y");
}
```