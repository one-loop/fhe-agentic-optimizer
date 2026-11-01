The product x0 * p needs one ciphertext-plaintext multiply, and the rescale after it is mandatory. I can't find a circuit with fewer operations. Every alternative I can think of adds cost:

- Splitting p into parts adds multiplies and adds.
- Squaring tricks need ciphertext-ciphertext multiplies, which cost more.
- Rotations are far more expensive than the 7 ms multiply.

Any circuit that differs from the baseline here will therefore be slower. I'm submitting the minimal form again, since I would rather not submit a deliberately worse kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```