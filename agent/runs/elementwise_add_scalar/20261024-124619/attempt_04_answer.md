The kernel is a single plaintext add, so there is nothing left to remove. The spread in timings (0.54, 0.38, 0.44 ms) comes from run-to-run noise on identical generated programs. I'm keeping the same kernel.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```