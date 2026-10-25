The kernel is already at the minimum: one plaintext add with no multiplications, rotations, or level consumption. The 0.45 ms vs 0.51 ms difference is timing noise, since the operation is identical, so I'm resubmitting it unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```