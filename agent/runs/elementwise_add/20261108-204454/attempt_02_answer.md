Nothing cheaper is possible here. The function needs exactly one ciphertext addition, and the baseline already uses only that. Any structurally different circuit has to add operations (an extra add, a rotation, a multiply), which makes it slower and can't pass the "≥3% faster" check. Spending the remaining attempts on deliberately worse circuits would only produce slower, losing results. The optimal kernel is the baseline:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```