I'm keeping the same kernel. Attempt 7 measured 18.05 ms, the best so far by a hair, with the same operation counts as every other run. The differences between attempts are timing noise, and the program already does the minimum work for `y = x * A + B`.

```cpp
// y = x * A + B. Minimal form: one ct-pt multiply (one level) plus one plaintext add.
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```