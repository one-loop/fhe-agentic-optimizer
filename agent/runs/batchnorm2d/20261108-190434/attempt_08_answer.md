This is the last attempt. It is the baseline again because no cheaper circuit exists for this task, so the final result stays at 3.73 ms.

The kernel already uses the fewest possible operations. It does one ciphertext×plaintext multiply, which CHEHAB follows with a rescale, then one plaintext add, using one level. The multiply is needed because `A` has a different value in each slot. Folding `B` in before the multiply would need plaintext-plaintext arithmetic, which is not allowed. Any other circuit that gives the same result only adds operations.

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