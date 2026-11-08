The baseline is still the cheapest circuit, so this attempt is the same kernel.

The output requires one ct-pt multiply by the runtime plaintext `s`. CKKS then needs one rescale, which CHEHAB inserts on its own. The circuit contains nothing else, so a structurally different one would only be slower. The baseline (1.66 ms, 1 level) is the final result for this task.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```