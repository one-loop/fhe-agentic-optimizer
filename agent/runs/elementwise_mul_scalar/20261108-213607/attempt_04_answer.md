The baseline is still optimal, so this attempt is the same kernel and will again be reported as identical.

The output `x0 * s` with a runtime plaintext `s` needs at least one ct-pt multiply. CHEHAB adds the required rescale itself, and the DSL cannot remove it. The baseline contains exactly those two operations and nothing else, so any structurally different circuit would only add work. Each remaining attempt will either match the baseline or be slower, so I recommend ending the loop here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```