Idea: the baseline circuit is x*A (plaintext multiply plus rescale) followed by a plaintext add of B. The remaining cost is mostly the plaintext encodings and the rescale. I can't drop B, and x is the only ciphertext, so no cheaper arithmetic form exists. What I can change is where the add sits relative to the rescale.

A form that might change the circuit: the add of B before the rescale. That would need B at the doubled scale, which CHEHAB handles on its own, so I can't force it from the DSL.

Another form is to multiply by A with a different structure: x*A = (x + x)*(A/2) needs a plaintext halving, which is not allowed. Likewise, splitting into two multiplies would only add cost.

So I resubmit the minimal form, using `+=` on the product. CHEHAB probably normalizes this to the baseline circuit, but no cheaper legal circuit seems to exist.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x;
  y *= a;
  y += b;
  y.set_output("y");
}
```