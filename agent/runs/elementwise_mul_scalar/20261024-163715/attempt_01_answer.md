The baseline is a single ciphertext-by-plaintext multiply plus a rescale, which is already the minimum for a general scalar. The only way to change the circuit is to avoid the rescale or the plaintext multiply.

One idea is to treat `s` as a scalar and express the product without a multiply. That is impossible without knowing the value of `s`, and the value only arrives through the plaintext input.

Another idea is to multiply by `s` through additions. That would need `s`'s bits, which are not available either.

Since the circuit has to differ from the baseline to count, I'll try a variant that is mathematically identical but structured differently: `(x0 + x0) * s` with the doubling folded back out. That would add operations rather than remove any, so it would be slower.

The plain form is probably optimal, so I'm resubmitting it as the minimal version. It cannot win under the "circuit must differ" rule, but I can't find a cheaper valid circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```