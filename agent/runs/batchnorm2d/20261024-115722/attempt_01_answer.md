The baseline is already minimal: one plaintext multiply, one rescale, and one plaintext add. The add costs about 2.5 ms for encoding B. Possible savings are limited because B must enter somehow, and every ciphertext-plaintext op requires encoding.

I'll submit the same computation unchanged, written so that CHEHAB sees the cheapest form. The only variation is the order of operations, which doesn't change the cost much. I'm keeping the baseline structure rather than adding operations that would only slow it down.

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