I have no cheaper circuit for the last attempt. `x0 + s` needs one add, and the baseline already has exactly that. Anything structurally different would add operations and run slower, so the baseline remains the best kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```