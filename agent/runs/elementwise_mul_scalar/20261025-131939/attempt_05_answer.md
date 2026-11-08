As expected, that was slower (18.60 ms against 8.81 ms): it used two plaintext multiplies and two rescales. The baseline is still the best I have.

I can't find a circuit with fewer than one plaintext multiply and one rescale. The scalar exists only as a plaintext input, so applying it needs a multiply, and CKKS then needs a rescale. The remaining attempts would only add operations, so I'm returning the baseline kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```