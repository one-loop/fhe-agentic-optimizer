The baseline is a single plaintext add, so there is little to remove. The cost is dominated by encoding the plaintext vector (~2.5 ms) plus the add. Any change that keeps the same operation counts is normalized away, so I'm submitting the minimal kernel unchanged in structure. I don't expect it to beat the baseline, but nothing cheaper is expressible: there are no multiplications, rotations or rescales to remove.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```