I have no cheaper circuit. The task is a single ciphertext + plaintext add, which is already the minimum: no multiplications, rotations, rescales or alignments. Anything that changes the circuit has to add operations, and those cost more than they save. So I'm submitting the same minimal kernel again, and it will probably be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```