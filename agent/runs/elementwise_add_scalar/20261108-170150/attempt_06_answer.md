The baseline is already a single add, which is the minimum for `x0 + s`, so I can't find a cheaper circuit. This is the baseline kernel again, and it will likely be reported as identical.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```