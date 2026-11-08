I still have no valid circuit that is cheaper than the baseline. I tried to make the circuit different and cheaper by hand, and every alternative needs at least one extra multiplication or a constant that CKKS doesn't allow:
- **`(x0+x1)*x1 - square(x1)`:** this is exact, but it uses two multiplications instead of one.
- **Polarization identities:** these need a 1/2 or 1/4 factor, and no plaintext input provides one.

Any slower variant would be rejected. The single ct-ct multiply is the optimum, so I'm keeping the baseline kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```