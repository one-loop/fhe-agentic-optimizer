I have no cheaper circuit to propose. The baseline's single plaintext multiply with one rescale is the minimum for this function, and the only structural variant I tried was slower (18.60 ms). I'm submitting the baseline kernel again, and I expect it to be reported as identical.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```