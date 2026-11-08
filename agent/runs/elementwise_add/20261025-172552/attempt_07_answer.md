The baseline is a single ciphertext addition, and no circuit with fewer operations can compute x0 + x1. Any circuit that differs from it must add work (extra negations, rotations, or multiplications), so it cannot be faster. Six attempts have confirmed that equivalent rewrites compile to the same circuit. I have no legitimate way to improve on 0.79 ms, so I'm submitting the minimal kernel again rather than a variant that only adds cost.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```