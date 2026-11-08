There is no cheaper circuit to find. The kernel is a single ciphertext-plaintext add, and every other valid form either reduces to it or adds operations. I've resubmitted it unchanged several times and it keeps matching the baseline circuit, so it can't be confirmed as a new best. Trying a deliberately different structure would only add cost, and I'd rather return a correct kernel than a slower one.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```