I don't see a cheaper circuit. One plaintext multiply with its rescale is the floor for this function, and one level is the minimum. The only change I can make is the operand order, `p * x0`, which CHEHAB will probably normalize to the same circuit, so I expect it to be rejected as identical again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = p * x0;
  y.set_output("y");
}
```