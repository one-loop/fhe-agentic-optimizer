No cheaper circuit exists. `y = x0 + p` needs at least one plaintext addition, and the baseline uses exactly that and nothing more. The baseline is the final answer for this task, and I am submitting it again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```