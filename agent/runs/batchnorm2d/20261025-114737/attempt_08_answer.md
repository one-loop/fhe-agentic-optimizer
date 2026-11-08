This is the last attempt, and I still can't find a circuit with fewer operations. The minimum for `y = x*A + B` with arbitrary plaintext vectors is one plaintext multiply with its rescale, plus one plaintext add. Getting below that would need a ciphertext of ones, a constant, or plaintext division, and none of those are allowed.

Every other form I tried only added cheap operations such as negations, and the one apparent speed-up was not confirmed in the alternating timing. So the baseline circuit remains the best. I'm submitting the minimal kernel.

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