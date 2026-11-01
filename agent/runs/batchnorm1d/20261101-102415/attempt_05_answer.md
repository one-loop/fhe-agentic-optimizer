This is the baseline kernel again. It will be judged identical and cannot win, and I don't see any kernel that can.

`y = x*A + B` already uses the minimal circuit: one plaintext multiply, the rescale CHEHAB inserts, and one plaintext add. The only factoring that could cut work needs `B/A`, which would require plaintext-plaintext arithmetic, and that is not allowed. Any other circuit adds operations, so it would only be slower. The 9.86 ms baseline is the final result for this task, and further attempts will just repeat it.

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