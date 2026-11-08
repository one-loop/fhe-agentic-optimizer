I can't find a cheaper circuit. One ciphertext-plaintext add is the least work that depends on both inputs. Any different circuit would need extra operations, so it would be slower or no faster than the baseline. I'm submitting the same kernel again rather than a variant that adds cost.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```