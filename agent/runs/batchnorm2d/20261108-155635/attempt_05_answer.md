Nothing here is cheaper than the baseline circuit, and the baseline form can't win again. For this attempt I'm negating the product instead of the input, `y = b - (-(x*A))`. This is the same cost class as attempt 3, so I don't expect it to beat the 3.77 ms baseline either.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext nt = -t;
  Ciphertext y = b - nt;
  y.set_output("y");
}
```