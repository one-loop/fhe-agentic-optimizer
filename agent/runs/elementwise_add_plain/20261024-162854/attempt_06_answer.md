My conclusion hasn't changed: I can't find a cheaper circuit than the baseline. It is one ciphertext-plaintext add, so it has no multiplications, rotations, rescales or alignments left to remove. Anything structurally different adds operations and is slower.

I'm submitting the baseline kernel once more. The 4.63 ms baseline remains the best result I can reach, and more attempts will not change that.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```