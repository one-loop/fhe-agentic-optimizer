I have no cheaper circuit to offer, and I'm not going to claim otherwise. The baseline already uses the minimum for `x0 * s`: one ciphertext-plaintext multiply and one rescale. Every valid rewrite I can think of either collapses back to this circuit or adds operations, so it could not beat the baseline. Resubmitting the same kernel again would only repeat attempts 1–3, so I'm stopping here. The baseline at 9.10 ms remains the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```