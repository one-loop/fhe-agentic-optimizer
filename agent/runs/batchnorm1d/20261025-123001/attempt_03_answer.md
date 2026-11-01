I don't have a legal rewrite that changes the circuit and lowers its cost. Any other form either needs plaintext-plaintext arithmetic or adds operations on top of the single plaintext multiply, rescale and add. The best valid kernel is still the baseline form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```