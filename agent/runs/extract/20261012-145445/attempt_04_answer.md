The best kernel is still the baseline at 29.17 ms. This attempt measured 29.47 ms with the same operation counts (one ct-pt multiply, one rotation, one rescale), so the spread across attempts is noise.

I'm not proposing another change. The kernel's cost is set by the two operations the task requires: one rotation to shift the window to slot 0, and one mask multiply to zero out the rest. Neither can be removed, and the current order already runs the rotation at the lower level.

```cpp
// Final kernel: one ct-pt multiply by the mask, then one rotation by slots()/4.

void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```