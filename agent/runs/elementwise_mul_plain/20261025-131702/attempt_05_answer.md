I have nothing new to try. The baseline, one plaintext multiply plus CHEHAB's automatic rescale, is the minimal circuit for `y = x0 * p`. Every different circuit I can build only adds operations, and attempt 2 showed that costs time. Resubmitting the same circuit just repeats the baseline, so I'm stopping here instead of using up the remaining attempts. The best result stays the baseline at 13.67 ms.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```