I have no cheaper circuit for the last attempt. The baseline's single plaintext multiply with rescale and single plaintext add are the minimum for `y = x*A + B`. My only structurally different attempt (negating around the multiply) was slower, at 18.71 ms against 17.90 ms. The other four resubmissions were identical to the baseline and were not timed.

The best kernel is still the baseline:

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