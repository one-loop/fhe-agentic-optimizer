I have not found a cheaper circuit that computes the same function, and I'm not going to submit another unchanged resubmission as if it were new.

What I tried and why it failed:
- **Split rotation (attempt 2):** two half-rotations gave 105 ms, much slower than the 59 ms baseline.
- **Rotating the mask (attempt 4):** the backend rejects rotating the plaintext, because it counts as plaintext-plaintext evaluation.
- **Rotating the input before the multiply:** this needs the rotated mask, so it is blocked for the same reason.

The baseline circuit (one plaintext multiply with rescale, then one rotation) is the minimum I can reach under the DSL rules. Any other form I can write either adds operations or changes the result. I'm keeping the baseline as the best kernel.

```cpp
void fhe()
{
  Ciphertext input("input");
  Plaintext mask("mask");
  const int offset = static_cast<int>(slots() / 4);
  Ciphertext output = (input * mask) << offset;
  output.set_output("output");
}
```