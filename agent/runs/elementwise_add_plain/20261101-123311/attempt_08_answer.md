For this last attempt I'm submitting the same kernel. A single add-plain is the cheapest circuit that can compute `y = x0 + p`, and any other structure would add operations and run slower. The baseline (2.72 ms) is therefore optimal for this task.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```