I have no cheaper circuit for this last attempt. The baseline's single ciphertext-plaintext add is the minimum for x0 + s, so I'm submitting it unchanged. The baseline stays the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```