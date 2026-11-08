I have no cheaper circuit to propose. The baseline is one plaintext add, and any structural change I can express adds operations. Attempt 2 showed this: its extra negations made it slower. I'm submitting the baseline kernel again, knowing it will be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```