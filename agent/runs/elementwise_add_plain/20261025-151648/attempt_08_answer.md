That result matches my expectation: the negation form is a new circuit, and the two extra negations cost about 0.4 ms (5.25 ms against 4.81 ms). The baseline remains the best.

With one attempt left, I still have no legal circuit that removes work from a single plaintext add. I'm resubmitting the baseline, which is the fastest circuit I know of, and I expect it to be judged identical again.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```