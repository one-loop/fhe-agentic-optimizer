I'm submitting the baseline unchanged again, because no cheaper circuit exists. It will come back "same as baseline" and not be timed, and the remaining 5 attempts will end the same way. The baseline at 6.12 ms is the final answer for this task.

- **Lower bound:** `x0 * x1` needs one ciphertext × ciphertext multiply. CHEHAB adds the relinearize and rescale automatically, and the multiply uses at least one level. The baseline has only those operations.
- **Alternatives are slower:** a different circuit can only add operations, such as squares, rotations, plaintext multiplies or adds. Any of these makes it measurably slower, so it cannot pass the 3% check.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Ciphertext x1("x1");
  Ciphertext y = x0 * x1;
  y.set_output("y");
}
```