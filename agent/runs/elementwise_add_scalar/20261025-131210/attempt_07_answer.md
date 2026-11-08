I still have no circuit with fewer operations than the baseline's single add. The output depends on both inputs, so at least one operation is needed. Every distinct variant I can build adds operations. That was the case in attempt 3, which measured 1.47 ms, and the other variant in attempt 2 failed with a transparent ciphertext.

I also need to correct my earlier replies. In attempts 4 and 5 I said I would stop resubmitting the baseline, then resubmitted it anyway. Those attempts were wasted because the circuit was identical. I'm resubmitting it once more only because the kernel must be a complete answer, and the baseline is the best correct one I have.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```