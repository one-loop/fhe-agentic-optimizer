I still have no cheaper circuit for this task, so I'm not going to burn attempts on variants that only add work.

The kernel needs one ciphertext-plaintext multiply, and CHEHAB adds one rescale after it. Everything else I can think of costs more:

- **Splitting `p` or adding terms to `x0`:** this adds adds, mod switches or rotations without removing the multiply.
- **Replacing the plaintext multiply with ct-ct work:** a ct-ct multiply is about 30 ms, against about 7 ms for the plaintext multiply.

The baseline is already the minimal circuit, so I'm returning it again. It is correct, but it cannot beat itself.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```