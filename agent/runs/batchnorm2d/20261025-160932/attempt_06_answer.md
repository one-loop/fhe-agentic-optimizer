I have no structurally different circuit that is cheaper. The function needs one plaintext multiply, which forces one rescale, and one plaintext add. The only other forms (negations, reordering, splitting the multiply) add operations or normalize back to the baseline. I'm keeping the baseline form, which is the minimal circuit.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```