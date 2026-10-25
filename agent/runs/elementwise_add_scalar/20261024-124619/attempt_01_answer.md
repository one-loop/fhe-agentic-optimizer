The baseline is a single ciphertext-plaintext add, which is already the minimum for this function. Any alternative (e.g. multiplying or rotating) would only add cost. The reported 0.55 ms is essentially the cost of one add plus overhead, so I'm keeping the kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```