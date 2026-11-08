No faster kernel exists for this task, so this is the baseline again, and the last two attempts will produce no improvement.

The circuit is one ciphertext×plaintext multiply, CHEHAB's automatic rescale and one plaintext add, using one level. The multiply is unavoidable because `A` varies per slot. Folding `B` in before the multiply would need plaintext-plaintext division, which is not allowed. Any other equivalent circuit only adds operations or levels.

The baseline at 3.73 ms is the final answer.

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