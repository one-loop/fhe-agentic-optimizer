The baseline does a plaintext multiply (with rescale) followed by one rotation. The two operations commute, so I can rotate first and then multiply, but that still costs the same operations. Reordering alone won't change the circuit.

Rotating first and then multiplying by a pre-rotated mask would be an option, but the mask is an input and I can't rotate a plaintext without a ciphertext operand. So the multiply and the rotation both remain.

The rotation is the more expensive operation. A rotation done at a deeper level is cheaper, so the multiply-then-rotate order the baseline already uses is likely the best one. After the multiply and rescale, the ciphertext sits at a lower level, so the rotation runs on a smaller modulus. The baseline therefore already has the cheap ordering.

I can't remove either operation, so I'll submit the equivalent form with the rotation first. It gives a different circuit, and the timing may still differ. It is a gamble, because the rotation then runs at the top level, which costs more. The baseline order is probably better, but I have no other structure to try on the first attempt, so I'll use this one to measure.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input << offset) * (mask << offset);
  output.set_output("output");
}
```

This kernel rotates the mask, which is a plaintext. If the compiler rejects that, I'll go back to the baseline form.