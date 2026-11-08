I don't have a legal circuit that is cheaper than the baseline, and I don't want to present a resubmission as a new idea.

- **Floor:** Every output slot depends on both `x0` and `p`, so one plaintext add (encode plus add) is the minimum.
- **Redundant identities:** CHEHAB normalizes them away, as the `x0 - x0` attempt showed. They produce the same circuit as the baseline and cannot win.
- **Deeper-level encoding:** Encoding `p` at a deeper level would be cheaper. Reaching a deeper level needs a multiplication, which costs about 5 ms and needs a constant I can't supply, or a mod switch, which I can't emit directly. Either way the cost exceeds the savings.
- **Other circuits:** Anything that survives normalization adds a multiplication, rotation, rescale or alignment, and each of those costs more than the roughly 3 ms add.

So I'm submitting the baseline kernel again. I expect it to be judged identical to the baseline.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 + p;
  y.set_output("y");
}
```