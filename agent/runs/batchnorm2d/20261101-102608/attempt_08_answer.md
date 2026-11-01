For the final attempt I am submitting the baseline again; it will be marked identical and not timed. In all 8 attempts I found no circuit cheaper than `x·A + B`, which uses one plaintext multiply, one rescale and one plaintext add. As explained earlier, any different circuit only adds operations.

The final result is the baseline at 9.91 ms.

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