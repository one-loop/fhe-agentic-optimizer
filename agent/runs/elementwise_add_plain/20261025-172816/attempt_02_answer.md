The baseline is already one plaintext encode plus one add, so nothing can be removed. This attempt rewrites the add as `-((-x0) - p)`. It is a structurally different circuit, but I expect it to be no faster, because the two negations are extra work. I don't see a cheaper circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext n = -x0;
  Ciphertext m = n - p;
  Ciphertext y = -m;
  y.set_output("y");
}
```