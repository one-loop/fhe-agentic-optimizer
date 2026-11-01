I am submitting the baseline kernel again (attempt 6), unchanged. I don't have a cheaper circuit and don't believe one exists.

`y = x*A + B` already uses the minimum: one plaintext multiply, the rescale CHEHAB inserts after it, and one plaintext add. The only factoring that could reduce work needs `B/A`, which is plaintext-plaintext arithmetic and is not allowed. Any other structure adds operations, so it would be slower. The baseline at 9.86 ms is the final answer for this task; repeating it for the remaining attempts won't change that.

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