A circuit with fewer than one operation can't produce `x0 + s`, so I don't see a cheaper circuit. The only alternatives add operations. Here I try the operand-swapped form `s + x0`, which may lower differently in CHEHAB. If it normalizes to the same circuit, it will again be rejected as identical.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = s + x0;
  y.set_output("y");
}
```