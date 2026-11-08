I have no cheaper circuit to propose. Six attempts have all reduced to the baseline's: one plaintext multiply and one rescale on a single level. For `y = x0 * p` that is the floor, with nothing left to cut. Any structurally different kernel would add operations, be slower, and so could not become the best.

The baseline stays the best kernel, and I'm submitting it unchanged and leaving the last two attempts unused.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```