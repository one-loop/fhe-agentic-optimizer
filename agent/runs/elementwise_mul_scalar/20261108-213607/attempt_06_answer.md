This resubmits the baseline unchanged, because no cheaper circuit exists.

The output needs one multiply by the runtime plaintext `s`, and CHEHAB adds the one rescale that CKKS requires after it. Nothing else is in the circuit, so any different structure can only add work. The remaining 3 attempts cannot produce an improvement, and the baseline at 1.66 ms is the final answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```