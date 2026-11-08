The baseline is already the cheapest circuit that computes `y = x0 * p`, and I'm submitting it again. One plaintext multiply with its rescale, at one level, is the minimum this function needs. There are no rotations, ct-ct multiplies, adds or alignments left to remove. Any differently structured kernel would only add operations, so I don't recommend using the remaining attempts.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```