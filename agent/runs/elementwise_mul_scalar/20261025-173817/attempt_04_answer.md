The circuit is already at its minimum, so I have no cheaper kernel to propose. `x0 * s` is one ciphertext-plaintext multiply with one rescale, and CHEHAB inserts that rescale itself. Every other form either simplifies back to this circuit or adds operations. I'm resubmitting the baseline kernel unchanged, and it will most likely be judged identical again. The baseline at 9.10 ms stays the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```