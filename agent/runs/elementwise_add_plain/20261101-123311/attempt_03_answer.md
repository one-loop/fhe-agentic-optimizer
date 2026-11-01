I'm resubmitting the baseline because no cheaper circuit exists.

The circuit is one add-plain and nothing else. Its cost is plaintext encoding plus one addition, and no other operation can produce `x0 + p`.

Any structurally different circuit adds operations, so it measures slower and fails the judging rule. That covers splitting `p`, adding then subtracting, extra rotations, or routing through a multiply.

The remaining attempts can't beat 2.72 ms; I'd suggest ending this task here.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```