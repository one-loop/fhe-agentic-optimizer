I could not find a cheaper circuit. `y = x*A + B` needs one plaintext multiply, which brings one rescale, and one plaintext add. Every algebraic rewrite I tried either needs plaintext-plaintext arithmetic (not allowed) or adds operations. Because CHEHAB normalizes reorderings, any kernel with the same operation counts is judged the same as the baseline. Below is the minimal form again.

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