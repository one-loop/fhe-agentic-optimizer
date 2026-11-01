This run took 2.70 ms, within the same 2.49–2.71 ms noise band as the other attempts. The kernel has no multiplications, rotations, or level use to remove, so there is nothing left to optimize. I'm submitting the same kernel as the best version (attempt 6), unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```