I'm keeping the same kernel. Attempt 4 measured 18.83 ms with the same operation counts as every previous run, so the spread between 18.06 and 19.28 ms is measurement noise. The best measurement so far is attempt 3 at 18.06 ms, and this program is the one I'd submit.

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