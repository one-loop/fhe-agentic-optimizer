I'm keeping the same kernel. Attempt 3 measured 18.06 ms with the same operation counts as the earlier runs (one ct-pt multiply, one rescale, one add, no rotations or ct-ct multiplies), so the change from 19.28 ms is measurement noise. The program is already the minimum for `y = x * A + B`.

```cpp
// y = x * A + B. Minimal form: one ct-pt multiply (one level) plus one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```