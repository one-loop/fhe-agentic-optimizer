I need to correct my last message. I said I was submitting `b + x * a`, but the code was identical to the baseline, so the 18.42 ms result was a repeat run and shows only measurement noise (18.14 to 18.42 ms across identical graphs).

The graph is already minimal. It has one ct-pt multiply with rescale and one plaintext add, and each plaintext input must be encoded and used once. Without constants or plaintext-plaintext arithmetic, I can't fold `b` into the ciphertext side or remove an operation. This attempt really does swap the operand order, `b + x * a`. I expect the same compiled graph and a time within noise.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y = b + y;
  y.set_output("y");
}
```