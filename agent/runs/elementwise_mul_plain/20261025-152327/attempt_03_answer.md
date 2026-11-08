I have no new circuit to try. The baseline is already minimal: one ct-pt multiply plus its rescale, with no rotations, ct-ct multiplies, adds or alignments. Any restructuring I can express under the CKKS rules adds operations rather than removing them. I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```