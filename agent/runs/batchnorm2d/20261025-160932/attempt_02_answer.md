The circuit already has one plaintext multiply, one rescale and one plaintext add, and I see no way to remove any of them. B/A would be plaintext-plaintext arithmetic, which is not allowed. This attempt adds B by subtracting a negated plaintext instead. It might change how CHEHAB handles the encoding of B and the scale alignment. If plaintext negation is rejected, the next attempt should go back to the baseline form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = x * a;
  Ciphertext nt = -t;
  Ciphertext y = -(nt - b);
  y.set_output("y");
}
```