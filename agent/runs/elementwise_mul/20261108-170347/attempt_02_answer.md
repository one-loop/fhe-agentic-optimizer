I can't find a cheaper circuit. Any different circuit I can build costs more than one multiply:

- **Square-based forms:** they need at least two squares plus a constant 1/4 or 1/2 plaintext multiply, which adds a level.
- **Other restructurings:** they add rotations or additions without removing the single multiply, relinearization and rescale.

So I'm resubmitting the minimal form. The judge will likely call it identical again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```