The baseline is the cheapest circuit for this task, so I'm returning it unchanged. There is no faster correct kernel to submit.

`y = x·A + B` takes one ciphertext×plaintext multiply, the rescale CHEHAB adds after it, and one plaintext add. The other routes are blocked or cost more:
- **Removing the multiply:** not possible, because `A` varies per slot and can only reach the ciphertext through a multiply.
- **Adding `B` first:** that would need `B/A`, which is plaintext-plaintext arithmetic and not allowed.
- **Any other equivalent circuit:** each one adds operations, at about 1–20 ms each against a 3.73 ms total.

You can end the run now and keep the baseline at 3.73 ms as the final result.

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