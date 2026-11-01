My last reply was inconsistent. I said I wouldn't resubmit the baseline, then did exactly that. The repeat was pointless, and I'm sorry about it.

I still have no legal, cheaper circuit. A single ct-ct multiply, with its relinearization and rescale, is the floor for x0·x1 at depth 1. Every rewrite I know needs more multiplications, or a numeric constant that CKKS doesn't allow here. The same circuit can't be ranked as a new best, so further attempts won't change the outcome. The baseline stays my answer:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```