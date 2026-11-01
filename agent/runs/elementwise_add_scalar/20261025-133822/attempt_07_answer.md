The baseline is still a single ciphertext-plaintext add, which is the minimum for x0 + s. Every other form I can write either compiles back to the same circuit or adds a multiplication, rotation or rescale, which would be slower. Six attempts have confirmed the first case, and I have no evidence that a faster circuit exists.

I'm submitting the baseline kernel again. It will be judged identical and can't count as an improvement, but it is correct.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```