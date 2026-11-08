I still have no cheaper circuit. `x0 + s` needs one add, and the baseline already has exactly that. Every attempt so far has produced the same circuit. I'm resubmitting the baseline form once more.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```