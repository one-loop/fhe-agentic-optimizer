The last attempt failed because CHEHAB folded `m - m` into a constant zero, which CKKS rejects. I'm dropping that trick.

The baseline already looks minimal: one ct-pt multiply with its rescale, and one plaintext add. A cheaper circuit has to remove one of those, and I don't see how to do that with only x, A and B as inputs.

One variant that might change the circuit is to add B to x before the multiply, which needs B/A and so a plaintext-plaintext division, which is not allowed. Another is to fold B into the ciphertext side with a ct-pt multiply by a plaintext, which adds work. So I'll try a form that moves the add before the rescale boundary by using `x * a + b` with the accumulation written as `+=`. CHEHAB probably normalizes it to the same circuit, but it is a valid, safe kernel.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```