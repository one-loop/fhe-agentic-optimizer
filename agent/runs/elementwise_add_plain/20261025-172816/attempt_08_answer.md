The baseline is still the best result. I don't have a faster kernel, and I should correct my last message. I described the attempt-7 kernel as adding a second plaintext add that cancels out, but it actually added a ciphertext add and a subtract. It reduced to the same circuit as attempt 2, which is why it was rejected.

No cheaper circuit exists for this function. `x0 + p` is one encode plus one ciphertext add, with no multiplications, rotations, rescales or alignments to remove. Every variant I can express adds operations (attempt 2 measured 5.29 ms against 4.64 ms) or is normalized back to the baseline. For the last attempt I'm submitting the baseline kernel, which cannot win, rather than a variant that is certain to be slower.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```