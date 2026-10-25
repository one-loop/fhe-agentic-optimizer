I'm stopping here. The kernel has one plaintext add and no other operations, so it is already at the floor for this task. Every run since the baseline has measured between 4.64 and 4.74 ms on the same work, which is noise, and I can't find a valid rewrite that does less than one add. Further attempts would just resubmit the same code.

```cpp
// Elementwise add plain: y = x0 + p. Already minimal (one add, no multiplications,
// no rotations, no level consumption); resubmitted unchanged.

void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```