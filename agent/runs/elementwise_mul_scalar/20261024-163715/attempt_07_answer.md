I still can't find a cheaper circuit than the baseline, and I haven't tried a structurally different one in the last several attempts. The baseline is one plaintext multiply and one rescale, with nothing else to remove. An alternative would have to add operations or be simplified back to the same circuit.

I'm submitting the same minimal kernel. It stays correct, but it will not beat the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```