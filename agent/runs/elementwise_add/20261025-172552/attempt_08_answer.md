The baseline is a single ciphertext add, which is the minimum for combining two ciphertext inputs. I can't find a cheaper circuit. Any different circuit would add operations and be slower, so I'm submitting the minimal kernel again as my final attempt.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 + x1;
  y.set_output("y");
}
```