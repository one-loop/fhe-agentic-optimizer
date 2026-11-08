I have no cheaper circuit. Every attempt so far either reproduced the baseline's circuit or added operations. One ciphertext multiplication is the floor for x², and with no constants or extra plaintext inputs there is nothing to trade it for. I'm keeping the baseline as the best and submitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x("x");
  Ciphertext y = x * x;
  y.set_output("y");
}
```