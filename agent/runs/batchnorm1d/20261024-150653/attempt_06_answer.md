I still have no structurally different circuit that is cheaper. Attempt 5 was another re-submission of the baseline form, and it confirmed that again. The remaining ideas all fail for the reasons in my last message:

- Pre-adding B needs a plaintext division.
- Constants are rejected.
- Any extra ciphertext operation makes the kernel slower.

The one circuit that computes the function correctly with the fewest operations is the baseline's: one ct-pt multiply with its rescale, plus one plaintext add. I'm keeping that kernel rather than spending the remaining attempts on rewrites that CHEHAB will normalize to the same circuit.

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