The baseline is a single plaintext-vector add, so the only cost is the plaintext encoding (~2.5 ms) and one ciphertext add. It needs no multiplications, rotations or rescales, and there is nothing left to remove from the circuit.

The one change I can try is the form of the operation. Writing `p + x0` or `x0 - (-p)` is normalized by CHEHAB into the same circuit, so it would not count as a new circuit. I'll submit the equivalent kernel with the operands swapped, accepting that it will probably match the baseline and not win.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```