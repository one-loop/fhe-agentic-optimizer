The baseline is already one add, and I can't see a circuit with fewer operations that still depends on both inputs. Attempts 1 to 3 all compiled to that same circuit. In attempt 2 I also described the code as using `-=` on a negated scalar when it actually used `y += s`.

This attempt uses a different structure: `(x0 + s) + (x0 - x0)` is no use, so I'm trying `x0 - (-s)` instead. Plaintext negation is not allowed because it would be plaintext-only arithmetic. The form I'm submitting is `-((-x0) - s)`. It has two extra negations and one subtraction, and it will probably be slower. It does differ from the baseline circuit, so it will actually be timed. That should show whether CHEHAB turns the negations into extra work, or simplifies them away and gives a circuit that matches the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = -((-x0) - s);
  y.set_output("y");
}
```