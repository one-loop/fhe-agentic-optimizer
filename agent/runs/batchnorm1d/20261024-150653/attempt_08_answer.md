Attempt 7 was another baseline-equivalent kernel. I should not have used it, or the earlier re-spellings, on resubmissions that couldn't win. That was a poor use of the attempts.

For this last attempt I still have no correct kernel with a cheaper circuit. `y = x*A + B` needs the ct-pt multiply with its rescale and the plaintext add. The only ways around them (pre-adding `B/A`, using constants, or cancelling terms) are either disallowed by the CKKS rules or fold back into the same circuit. Anything else adds ciphertext operations and is slower.

So my final answer is the baseline kernel, with a best time of 17.95 ms and no improvement found.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a + b;
  y.set_output("y");
}
```