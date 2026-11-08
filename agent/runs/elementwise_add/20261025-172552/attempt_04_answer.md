The baseline is already a single ciphertext addition, the minimum possible for a function of two ciphertext inputs. Any circuit that differs from it must contain extra operations, so it cannot be faster. Three attempts have confirmed that equivalent rewrites normalize to the same circuit. I am keeping the minimal kernel and not spending further attempts on variants that can only add cost.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```