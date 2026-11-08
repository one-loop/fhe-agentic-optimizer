The task is a single plaintext-vector multiply, which is already the minimum for computing x0 * p. The one operation is a ct_pt_mul plus its rescale, and there is no cheaper way to express it under the CKKS rules. Here is the baseline unchanged, in case no variant can beat it.

A different circuit would need a different operation mix. Computing x0*p as (x0+x0)*(p/2) would need constants and extra operations, and it would add cost instead of removing any. I therefore submit the minimal form.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```