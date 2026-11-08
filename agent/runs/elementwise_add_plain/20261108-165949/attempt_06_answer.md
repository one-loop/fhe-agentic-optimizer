I have no cheaper circuit to propose. The task is a single plaintext add, which is already the minimum. Five attempts have returned the identical circuit, and any structurally different version would add operations. The baseline stays the best kernel, and I'm submitting it again unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```