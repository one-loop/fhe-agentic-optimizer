Attempts 3 and 7 show that these negation variants only add operations and stay within noise of the baseline. I have no cheaper circuit to propose for the last attempt. `y = x*A + B` needs one plaintext multiply with its rescale and one plaintext add. The baseline already does exactly that, so I'm submitting it unchanged as my final answer.

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