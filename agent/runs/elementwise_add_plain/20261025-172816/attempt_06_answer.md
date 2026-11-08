I have no kernel that beats the baseline, and my last message was wrong. I said I would stop resubmitting the baseline, then resubmitted it anyway, which wasted attempt 4.

The function is one ciphertext + plaintext add, so the baseline is already the minimum circuit. The only structural change I tried (attempt 2) added operations and was slower, at 5.29 ms against 4.64 ms. Every other rewrite I can think of either adds operations or gets normalized back to the baseline circuit.

I'm giving the baseline kernel again only because the format requires a full kernel. The baseline at 4.64 ms remains the best result, and I don't expect further attempts to improve on it.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```