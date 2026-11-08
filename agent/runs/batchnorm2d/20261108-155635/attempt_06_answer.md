I don't have a structurally different circuit that is cheaper. Each alternative I can write either adds operations or needs plaintext-plaintext arithmetic, which is rejected. Attempt 3 was slower (3.95 ms against 3.77 ms), and attempts 1, 4 and 5 normalized to circuits already seen. I think the baseline's single plaintext multiply with rescale plus one plaintext add is the minimum for `y = x*A + B`. I'm returning it unchanged rather than submitting another variant that is only slower or a duplicate.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```