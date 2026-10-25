Every operation in the baseline is needed: y = x*A + B takes one ct-pt multiply, one rescale and one add. To change the circuit I need a structurally different way to compute it.

Idea: the ct-pt multiply and its rescale cost about 7 ms, and the add of B costs about 2.5 ms for encoding. If B is added to a ciphertext at the same level and scale as the product, CHEHAB encodes B at the product's scale. Adding B first would change the scale pattern: (x + B') * A needs B' = B/A, which requires plaintext-plaintext division and is not allowed.

A cheaper variant is to avoid the multiplication's rescale by making the add happen at a different level. For example, compute x*A without a rescale and add B at the unrescaled scale. CHEHAB decides rescale placement itself, so I can't control that directly.

I can try an algebraic restructuring: y = (x + x*A) + (B - x), which adds operations rather than removing them. That is worse.

Another option is to use a ciphertext-ciphertext form with the plaintext as an operand. That is more expensive too.

I'll try a form where the add is done as a subtraction of a negated plaintext. The operation counts would be the same, so this probably won't change the circuit, but it is the only structural variant that remains.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext y = m - (-(m - m) - b);
  y.set_output("y");
}
```

Note that `-(m - m)` is a ciphertext, so `... - b` is valid. CHEHAB's simplifier will probably fold `m - m` to zero and the expression back to `m + b`. If it does, this will again match the baseline circuit.