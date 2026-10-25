The identical kernel measured 1.71 ms versus 0.72 ms, so the difference is timing noise, not a change in the program. The generated operations are the same (one ciphertext add). A single add is the minimum for this function, and there is nothing to remove, so I'm keeping the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```