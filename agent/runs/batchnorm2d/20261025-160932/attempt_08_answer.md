I have no new idea for a cheaper circuit in this last attempt. The baseline circuit has the minimum operation set (one plaintext multiply, one rescale, one plaintext add), and each variant I tried normalized to it or added operations. I'm leaving the baseline form as my answer.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```