No cheaper circuit exists. The kernel is already one plaintext addition with zero levels, multiplications, rotations or rescales. Every output needs both `x0` and `s`, so one addition is the minimum. Any structurally different circuit would cost more and could not pass the "confirmed faster" check.

I'm returning the optimal kernel unchanged. The remaining attempts can't produce an improvement.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```