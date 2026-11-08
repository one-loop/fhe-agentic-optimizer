I need to be honest about attempt 2: the kernel I submitted did not contain the redundant subtraction identity I described. It was just the baseline again, so my description did not match the code.

The baseline is one plaintext add, which is the minimum for this function. Every output slot depends on both inputs, so at least one operation touching `p` is required. Any circuit that differs from the baseline has to add operations, such as extra adds, subtractions, or alignments, and that cannot be cheaper. I do not see a legal way to cut the cost of the encode and add.

The remaining variants are all redundant ones, for example computing `x0 + p` as `(x0 + p) + (x0 - x0)`. They only add ciphertext adds of about 0.5 ms each, so I expect them to be slower. I'll try the cheapest such variant to check how much the extra operations cost. It has one extra ciphertext subtraction and one extra add.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext z = x0 - x0;
  Ciphertext y = (x0 + p) + z;
  y.set_output("y");
}
```