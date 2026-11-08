I still can't find a valid circuit with fewer operations than the baseline. The one ciphertext-plaintext multiply and its rescale are the minimum for `x0 * s`. Any rewrite either gets normalized back to this circuit or adds work, such as a second plaintext multiply or extra alignment. I'm not going to submit a deliberately slower variant, so the baseline stays the best. The kernel below is the same minimal one.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```