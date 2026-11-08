I found no kernel faster than the baseline, and this last attempt is the same minimal one. The baseline computes `x0 * s` with a single plaintext multiply and its rescale, which is the least work possible here. Every other circuit I can write adds operations, so it could only be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```