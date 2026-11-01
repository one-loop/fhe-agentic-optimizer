The baseline is already the minimal circuit for this function: one plaintext addition (~2.5 ms, almost all of it plaintext encoding), with no multiplications, rotations, rescales or level changes. `y = x0 + p` needs at least one operation that combines the ciphertext with the plaintext. CKKS has no way to fold `p` into `x0` without that add, and computing it any other way, for example through a multiply by a constant, would add operations and be slower.

So there is no faster kernel to propose. A variant would have to change the circuit to be judged at all, and every possible change makes it more expensive. I'm returning the optimal kernel unchanged:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```