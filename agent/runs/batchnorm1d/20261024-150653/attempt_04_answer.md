I have to be upfront about the earlier attempts: two were just re-spellings of the baseline, which CHEHAB normalizes to the same circuit, and the other failed because `m - m` folded to a constant. None of them could have won.

The baseline already has the minimum operation set for y = x*A + B with these inputs: one ct-pt multiply, one rescale and one plaintext add. The only change that could alter the circuit is to move the plaintext add to a different place in the scale/level pattern.

Idea: multiply the ciphertext by A as usual, but make the add structurally different. Computing `(x + x) * a` or splitting A adds operations, so it can't be cheaper. A real alternative is to turn the plaintext add into a ct-pt multiply-free form, but B has no ciphertext partner, so it needs an encode either way.

I'll try one structurally different candidate. Adding B to a ciphertext at the same level before the multiply would require B/A, which isn't available. So I split the work: `x*a` gives the scaled term, and B is added after it, with a second ct-pt multiply replaced by nothing. This is the same as the baseline, so I can't promise a different circuit.

The remaining option is a redundant-looking form that CHEHAB might lower differently: multiply by `a`, then add `b` through a ciphertext built from `x`. Building one means multiplying x by a plaintext, which costs another multiply and so is slower.

I don't have a kernel I expect to beat the baseline, so I'm resubmitting the baseline form rather than a variant that would only be slower. I would rather say that than present a rewrite as an improvement.

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