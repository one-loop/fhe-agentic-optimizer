The baseline circuit is already minimal. This attempt uses a structurally different form that negates `x` first and then subtracts, which should change the operation counts. I expect it to be equal or slightly slower than the baseline, so it is mainly a probe of how CHEHAB handles the negation.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext nx = -x;
  Ciphertext t = nx * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
```