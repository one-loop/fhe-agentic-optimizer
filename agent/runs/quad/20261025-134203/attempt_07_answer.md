That repeat was my mistake. I resubmitted the baseline circuit, so it could not be faster and used up an attempt.

I have no valid idea for a cheaper circuit. The few remaining ones are all worse:

- **Second multiply:** attempt 3 doubled the time.
- **Cancelling terms:** CHEHAB removes them and returns the baseline circuit.
- **Lower-level multiply:** a multiply is cheaper at a deeper level, but I can't drop `x` to a lower level without spending a multiplication or having a plaintext input.
- **Scaled forms:** `square(x+x)` gives 4x², and fixing that needs a constant.

I'm leaving the last two attempts unused instead of submitting variants I expect to be slower or identical. The baseline form remains the best:

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```