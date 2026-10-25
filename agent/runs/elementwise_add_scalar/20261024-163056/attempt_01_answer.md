The baseline is already a single ciphertext-plaintext add, so there is little room to improve. The remaining cost is probably encoding the scalar plaintext into a full vector, and I can't see a way to avoid that, since every operation needs the plaintext `s` as an operand. Any alternative, such as a multiplication or a rotation, adds work. I'm resubmitting the same minimal circuit, which probably won't count as a new best because the circuit is unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```