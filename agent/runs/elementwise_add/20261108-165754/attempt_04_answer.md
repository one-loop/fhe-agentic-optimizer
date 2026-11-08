There is still nothing cheaper to try. The kernel needs exactly one ciphertext add, and that is already the whole circuit. Every alternative I can think of (adding zero, splitting the add, reordering operands) either adds operations or gets normalized back to the same circuit. I'm submitting the same kernel again, so it will match the baseline and can't be ranked faster. I'd rather say that plainly than invent a change that only makes it slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```