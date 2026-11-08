The circuit is already a single add with plaintext encoding, so there is no cheaper circuit. Any variant that changes the circuit would add operations, because every extra operation costs more than it saves. One possible gain is the plaintext encoding at ~2.5 ms. It depends on the input itself, and the DSL gives no way to skip it, since a plaintext input must be encoded to be added.

I'm resubmitting the minimal kernel. I expect it to match the baseline and can't offer a cheaper circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```