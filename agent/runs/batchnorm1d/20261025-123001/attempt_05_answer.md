This variant uses one negation instead of two: compute (−x)·A and then B − (−x·A). It is still a legal rewrite, with a single plaintext multiply and one plaintext add or subtract.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = (-x) * a;
  Ciphertext y = b - t;
  y.set_output("y");
}
```