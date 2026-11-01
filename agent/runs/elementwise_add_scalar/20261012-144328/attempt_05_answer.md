I'm submitting the same kernel again. Across attempts the timing ranges from 0.25 to 0.27 ms for the identical single ciphertext-plaintext addition, so these differences are measurement noise, not changes in the code.

The kernel does the minimum possible work: one add, no multiplications, rotations, rescales, or level consumption. I don't see a legitimate way to reduce that further under the stated rules.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 + s;
  y.set_output("y");
}
```