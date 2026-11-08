I still have no cheaper circuit. One plaintext multiply and one rescale is the floor for `x0 * s`, and the one structurally different variant I tried (two plaintext multiplies) took 18.60 ms. Resubmitting the baseline again would just be reported as identical, so I'm not going to pretend the next attempt is a new idea. The best kernel remains the baseline:

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```