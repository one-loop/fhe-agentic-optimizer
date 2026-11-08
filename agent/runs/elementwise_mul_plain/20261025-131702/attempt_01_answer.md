The baseline is already one plaintext multiply plus one rescale, so there is little room to cut operations. Any equivalent form needs the same multiplication by `p`. The one circuit change I can think of is to skip the multiply and compute the product another way, but without constants or plaintext-plaintext arithmetic nothing cheaper exists.

I'll submit a variant of the baseline that may differ in how CHEHAB normalizes it. It computes `x0 * p` as `(x0 + x0) * p` minus `x0 * p`. That adds a multiply, so it should be slower, and I won't use it. The plain multiply is the minimal circuit, so I'm resubmitting it as the best known form.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```