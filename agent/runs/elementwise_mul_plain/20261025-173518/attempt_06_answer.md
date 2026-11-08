The baseline's single ct-pt multiply plus rescale is the minimum for x0 * p, and every different circuit so far has added operations (attempt 5 added two adds and measured 13.99 ms against 13.05 ms). I can't find a correct circuit with fewer operations than the baseline, so I'm resubmitting it. It is the best known kernel and should be treated as the final answer unless a cheaper idea turns up.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```