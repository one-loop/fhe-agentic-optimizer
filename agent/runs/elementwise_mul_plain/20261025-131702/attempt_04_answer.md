I don't have a cheaper circuit to offer. Every correct kernel I can build needs the one plaintext multiply and its rescale, which is exactly what the baseline does. Anything else adds operations, as attempt 2 did.

The remaining ideas don't work:
- **Fold the multiply into other operations:** the rules ban numeric constants and plaintext-plaintext arithmetic.
- **Use rotations or ct-ct multiplies:** these cost 20-30 ms each.
- **Add or subtract tricks:** the output is a single product, so they can only add ciphertext additions.

I'm resubmitting the baseline circuit, since I can't improve on it. It will not be timed again, and the baseline stays the best result at 13.67 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```