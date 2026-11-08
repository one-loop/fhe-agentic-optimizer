The baseline's `x * x` is a ct-ct multiply, which CHEHAB counts as ct_ct_mul. Using `square(x)` is the same mathematical operation, but it can be implemented more cheaply: the squaring avoids some of the NTT work in the tensor product. It also changes the operation counts (square=1 instead of ct_ct_mul=1), so the circuit differs from the baseline and can be ranked.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```