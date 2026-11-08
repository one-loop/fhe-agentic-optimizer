I still see no cheaper circuit, so this attempt is a different structure that I expect to be slower, not faster. It computes `(x0+x0)*s - x0*s`, which equals `x0*s` but uses two plaintext multiplies. I'm submitting it only to see whether CHEHAB simplifies it into something different from the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext a = x0 + x0;
  Ciphertext b = a * s;
  Ciphertext c = x0 * s;
  Ciphertext y = b - c;
  y.set_output("y");
}
```