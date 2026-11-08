I still have no cheaper circuit, so this is the same kernel again. Resubmitting it will keep returning "identical to the baseline", and I'd rather not burn the remaining attempts on variants that are strictly worse.

`x0 * x1` is the floor for this task: one ciphertext-ciphertext multiply, one relinearization and one rescale. The ways I considered to change the circuit all add cost or break correctness:
- **Square identities:** they give 2ab or 4ab, and removing the factor needs a constant that no plaintext input supplies.
- **Extra mod-switches or alignments:** they only add operations.
- **Reordering or regrouping:** CHEHAB normalizes these away, so the circuit stays identical.

Unless the task changes, for example by adding a plaintext input or letting me skip relinearization, the baseline is the best achievable circuit.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```