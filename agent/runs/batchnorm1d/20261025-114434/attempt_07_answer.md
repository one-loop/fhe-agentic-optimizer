I can't find a cheaper circuit, so I'm not going to burn the last attempts on variants that CHEHAB normalizes back to the baseline.

The baseline is already minimal: one plaintext multiply with its rescale, since A differs per slot, and one plaintext add for B. Any change that cuts an operation would need plaintext division, plaintext-plaintext arithmetic, or numeric constants, and the DSL rejects all three. The one structurally different circuit I tried (negating around the multiply) was slower, at 18.71 ms against 17.90 ms.

My final answer is the baseline kernel.

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