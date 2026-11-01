The timing change from 9.84 ms to 9.79 ms is run-to-run noise, since the operation counts are identical. I see no algebraic rewrite that does less work. Folding the shift into the multiply would need `B/A`, which requires plaintext-plaintext arithmetic (forbidden), and no new plaintext inputs are allowed. Splitting the work across levels doesn't help either, because there is only one multiply. I'm keeping the kernel as it is.

```cpp
// y = x * A + B: one ciphertext-plaintext multiply (depth 1) plus one plaintext add.
// Minimal evaluation for this function; no further reduction in operations is possible.

void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```