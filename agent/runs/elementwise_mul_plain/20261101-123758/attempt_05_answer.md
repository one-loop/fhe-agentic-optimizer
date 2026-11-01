I'm resubmitting the baseline because no cheaper circuit exists. Computing `x0 * p` with a run-time plaintext vector `p` takes at least one ct-pt multiply, and CHEHAB adds the rescale after it automatically. The baseline already has nothing else. Any structurally different kernel is this circuit plus extra operations, so it would be slower. The remaining attempts cannot beat 7.06 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```