I still have no cheaper valid circuit, and I'm not going to claim one. The single ct-ct multiply is the minimum for `x0 * x1`.

Every exact alternative I can construct either:
- adds multiplications (for example `(x0+x1)*x1 - square(x1)`), or
- needs a numeric constant such as 1/2 or 1/4, which CKKS forbids and no plaintext input provides.

Both are slower or invalid. I'm resubmitting the baseline kernel because I have nothing better. Further attempts would only repeat the same circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```