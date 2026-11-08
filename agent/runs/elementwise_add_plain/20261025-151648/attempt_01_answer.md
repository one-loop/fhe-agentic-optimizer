The baseline is a single plaintext-vector add, which costs about 2.5 ms for encoding plus 0.5 ms for the add. The computation cannot be simplified algebraically, because every output slot depends on both `x0` and `p`.

The only alternative is to change which operations run. Computing `-(-x0 - p)` would add operations and cost more. Subtracting the negated plaintext is not possible either, since negating a plaintext is plaintext arithmetic, which is not allowed. So the minimal circuit is the baseline, and I resubmit it unchanged. It should be judged as no improvement, but I see no valid circuit that is cheaper.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```