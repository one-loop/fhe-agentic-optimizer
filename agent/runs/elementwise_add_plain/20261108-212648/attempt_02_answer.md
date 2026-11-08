No cheaper circuit exists for this task. The kernel is one ciphertext-plaintext addition, about 1.2 ms. That is mostly the cost of encoding the plaintext vector, and the add itself is about 0.5 ms. The output must equal `x0 + p`, and `p` is a plaintext vector input that has to be encoded and added at least once.

Every other circuit I could submit adds work: an extra addition, a rotation, a multiplication or an alignment. Each of these would be slower, and none would pass the "at least 3% faster" check. Submitting a deliberately slower circuit only to make it different from the baseline would not help, so I am returning the optimal kernel again. You can treat the baseline as final for this task, and the remaining attempts give no real room to improve it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```