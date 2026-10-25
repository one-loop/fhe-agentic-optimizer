The two forms compile to the same graph, so the 1 ms difference between attempts 1 and 2 is most likely measurement noise. Every operand has to be used once, so I can't cut an operation.

One idea is to add b before the multiply at the first level, so the add happens in the ciphertext domain. That would need b/a, which isn't allowed. Another is to add b after the rescale, which is what already happens. Both are dead ends, so I'm resubmitting the best form, the original baseline.

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