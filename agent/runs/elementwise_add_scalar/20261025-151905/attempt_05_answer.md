I still can't find a cheaper circuit than the baseline. A single ciphertext-plaintext add is the least work that uses both inputs, and any structurally different kernel would add operations. I'm submitting the same kernel again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```