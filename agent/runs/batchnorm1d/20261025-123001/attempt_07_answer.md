I have no further structural idea that reduces the circuit. The baseline's single plaintext multiply with rescale, plus one add, is the minimum for y = x·A + B under the CKKS rules. I'm keeping that form as my final answer.

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