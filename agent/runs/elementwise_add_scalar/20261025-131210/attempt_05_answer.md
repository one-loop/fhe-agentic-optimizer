I have no cheaper circuit to offer. The baseline is one ciphertext-plaintext add, which is the minimum, because the output depends on both inputs and so needs at least one operation. Every variant I can build from the allowed operators either keeps the same circuit, which cannot win, or adds operations, which was slower (1.47 ms in attempt 3). I'm submitting the baseline kernel again as the best known answer.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```