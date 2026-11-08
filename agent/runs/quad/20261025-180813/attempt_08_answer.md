I still have no cheaper circuit. The baseline is already at the minimum of one ciphertext multiplication, and everything I tried either collapsed to the same circuit or added operations.

For the last attempt I'll go back to the `square(x)` form from attempt 1. It was a distinct circuit that measured 67.70 ms against 65.95 ms in a single timing, which is within run-to-run noise but was not confirmed faster.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = square(x);
  y.set_output("y");
}
```