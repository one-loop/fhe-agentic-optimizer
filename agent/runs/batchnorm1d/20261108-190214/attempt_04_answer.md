The baseline kernel is the fastest possible, so I'm returning it unchanged.

The task needs exactly one multiply by A and one add of B, and the baseline already does only that. Any circuit different from it has to add at least one operation, and every operation costs time. Writing the expression differently also doesn't help, because CHEHAB rewrites equivalent forms back to the same circuit. That is why attempts 1–3 all came out identical.

The remaining 5 attempts can't produce a winner, so there's no point in using them.

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