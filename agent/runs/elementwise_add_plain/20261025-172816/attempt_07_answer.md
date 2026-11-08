I made the same mistake again: attempt 5 resubmitted the baseline right after I said it was wasted. The result was the same, because the circuit is identical to the baseline and can't win.

I don't have a cheaper circuit. The function is one ciphertext + plaintext add, and the only alternative I tried (attempt 2) added operations and measured 5.29 ms against the baseline's 4.64 ms. Any further rewrite would either add work or be normalized back to the baseline circuit.

For the next attempt I'll submit a circuit that differs from the baseline. It will almost certainly be slower, since it adds a second plaintext add that cancels out, and it's only worth trying if you want to see whether CHEHAB's timing shows any surprise. The baseline at 4.64 ms remains the best result.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext t = x0 + p;
  Ciphertext y = t + x0 - x0;
  y.set_output("y");
}
```