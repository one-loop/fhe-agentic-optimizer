I am submitting the baseline a sixth time; it will be marked identical and not timed. As explained in my earlier answers, `x·A + B` is already the minimal circuit, and every alternative only adds operations.

The remaining 3 attempts cannot improve on it. I recommend stopping with the baseline (9.91 ms) as the final result.

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