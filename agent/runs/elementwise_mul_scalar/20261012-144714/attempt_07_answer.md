The kernel stays the same. Attempt 6 measured 4.60 ms, and the results across attempts range from 4.58 to 4.60 ms, which is run-to-run noise. The kernel already uses the minimum work: one ciphertext-plaintext multiply and one rescale, with no rotations or ciphertext-ciphertext multiplies, and depth 1.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```