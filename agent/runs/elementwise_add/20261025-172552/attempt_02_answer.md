The circuit is already a single ciphertext add, so no cheaper circuit exists. Any structurally different version would add operations, because the output needs both inputs and there is no way to combine them with fewer than one operation. Alternatives like `x0 - (-x1)` or `(x0 + x1)` routed through extra ops only add cost. I'm keeping the minimal kernel, since a different circuit can't be faster here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```