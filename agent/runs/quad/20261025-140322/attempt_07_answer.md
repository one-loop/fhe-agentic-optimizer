Attempt 6 was folded back to the plain square: CHEHAB simplifies `x - x` to zero and removes the add. So the extra-add variants only survive when the added terms are not exactly cancelling.

I still have no way to make the circuit cheaper. This attempt uses the same x² computation as attempts 4 and 5, but with two differently grouped copies of x as the multiply operands. It should land at the same cost as them, and I don't expect it to win.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext a = (x + x) - x;
  Ciphertext b = (x + x + x) - (x + x);
  Ciphertext y = a * b;
  y.set_output("y");
}
```