I don't see a cheaper circuit for this task. The output needs one ciphertext-plaintext multiply, and CHEHAB adds the one rescale that goes with it. Every alternative I can think of adds operations:

- **Splitting `p` or pre-adding `x0`:** this adds rotations, adds, or extra multiplies, and CKKS gives me no way to fold in a constant.
- **Mod-switching `x0` first:** this adds a mod switch and does not shorten the multiply.
- **Squaring-based identities:** a ct-ct multiply costs about 30 ms, far more than the 7 ms plaintext multiply.

So the baseline circuit looks minimal. I'm resubmitting it unchanged, which means it can't win the comparison.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```