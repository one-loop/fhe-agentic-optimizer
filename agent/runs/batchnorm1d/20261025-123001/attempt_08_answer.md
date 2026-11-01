I found no cheaper circuit for this task. Seven attempts either reproduced the baseline circuit or, in attempt 4, added operations and measured 10.07 ms against the baseline's 9.68 ms. For y = x·A + B, the baseline's one plaintext multiply (with its rescale) and one add is the minimum. Any cheaper form would need plaintext-plaintext arithmetic, such as B/A or A·B, and CKKS forbids that.

I'm submitting the baseline form for the last attempt.

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