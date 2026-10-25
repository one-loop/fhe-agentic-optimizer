This run took 19.90 ms with the same code as the 18.14 ms run, so the spread is noise from the machine, not from the kernel. The graph is still 1 ct-pt multiply, 1 rescale and 1 add, and I have no valid rewrite that reduces it. I'm submitting the baseline expression again as the best-measured form.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```