I still can't find a cheaper circuit. The multiply and its rescale are unavoidable, and the only other cost is the single plaintext add. Every rewrite I can think of either needs a constant or plaintext-plaintext arithmetic, which CKKS rules 1 and 2 forbid, or adds operations (as in attempt 2). I'm resubmitting the minimal form, which is identical to the baseline circuit.

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