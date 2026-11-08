The baseline `x * x` is compiled as a ct_ct_mul with relin and rescale. The task statement counts `square(c)` as a ciphertext-ciphertext multiplication, but it is a distinct operation in the circuit and may be cheaper in SEAL, since squaring needs fewer NTT operations. It also changes the circuit signature (square=1 instead of ct_ct_mul=1), so it counts as a different circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```