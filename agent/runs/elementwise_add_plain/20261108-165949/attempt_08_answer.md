The baseline is already the cheapest circuit for this task, and I can't find a cheaper one. It uses one plaintext add and nothing else: no multiplications, rotations, rescales or levels. Seven attempts have confirmed the same circuit, and any different structure would add operations. I'm submitting the baseline kernel one last time.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```