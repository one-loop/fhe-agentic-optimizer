The baseline does one plaintext multiply with a rescale, plus one plaintext add (which needs encoding). The cost is dominated by the ct-pt multiply (~7 ms) and its rescale, and by the plaintext encode for B.

The function y = x*A + B needs one multiplication by a vector, so it cannot avoid that multiply. The options are to change the circuit's shape:

- Adding B before the multiply would need B/A, which is plaintext-plaintext arithmetic and not allowed.
- Since B is added at the post-multiply scale, the plain add must encode B at that scale. That encoding is unavoidable.

One alternative is to reorder the operations as (x + B') * A, but B' = B/A cannot be computed. So the circuit stays essentially the same. I will try a variant that differs structurally: compute x*A as a ciphertext product and add B via a subtraction of a negated term. That is equivalent after normalization and is unlikely to help, but it is the only variation available without changing the math.

Nothing clearly cheaper exists, so I resubmit the baseline form, which is already minimal.

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