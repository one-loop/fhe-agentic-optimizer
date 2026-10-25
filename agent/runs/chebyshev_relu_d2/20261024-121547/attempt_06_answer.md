I found no faster kernel, so this is the best-known one again (81.55 ms, down from the 107.23 ms baseline). A different structure would need extra plaintext multiplies or more levels. For example, multiplying the inner term by `x` instead of `z` would need another alpha factor, which costs an additional ct-pt multiply.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext alpha("alpha");
  Plaintext c0("c0");
  Plaintext c1("c1");
  Plaintext c2("c2");
  Ciphertext z = x * alpha;
  Ciphertext w = z * c2;
  Ciphertext s = w + c1;
  s = s + w;
  Ciphertext r = z * s;
  r = r - c2;
  r = r + c0;
  r.set_output("y");
}
```