The 1.47 ms result confirms that extra adds only slow things down. The baseline is one ciphertext-plaintext add, and every other form I can build from the allowed operators has at least as many operations. I don't see a circuit with fewer operations than one add, so I don't expect any remaining attempt to beat the baseline.

Any circuit that differs from the baseline has to add operations, because the output depends on both inputs and so needs at least one operation. A cheaper single operation does not exist. A valid circuit that is also faster is therefore unlikely.

I'm submitting the baseline kernel again as the best known answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```