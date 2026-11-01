This kernel is already the minimum: a single plaintext add with no multiplications, rotations, or level use. The 2.69–2.71 ms spread is measurement noise, and the remaining cost is the plaintext encoding, which can't be avoided because `p` is a runtime input. I'm keeping the best version from attempt 1.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```