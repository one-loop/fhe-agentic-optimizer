I'm resubmitting the same kernel, because a single add-plain is already the minimal circuit for `y = x0 + p`. Any structurally different circuit would add operations and run slower, so 2.72 ms can't be beaten. More attempts won't change that, so I recommend ending this task.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```