The circuit is already one ciphertext add, and no cheaper form exists. Two inputs need at least one operation to combine, and the rules forbid emitting anything that would skip it. I can't see a legitimate rewrite that removes that add without changing the output.

Any variant I could try would either normalize back to the same circuit, which cannot win, or add operations and run slower. So I'm keeping the minimal kernel instead of padding it with extra work.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```