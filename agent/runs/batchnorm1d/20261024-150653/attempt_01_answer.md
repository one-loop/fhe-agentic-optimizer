The baseline already uses a single ct-pt multiply, one rescale and one plaintext add. The only way to change the circuit is to avoid one of those operations.

Idea: the plaintext add (B) costs about 2.5 ms for encoding. Adding B before the multiply is not possible without dividing by A. The alternative is to encode B once at the same scale as the product, but CHEHAB already does that.

A real change is to add B after the rescale rather than before it. Since rescale is applied lazily, the baseline probably adds the plaintext at the product's scale and rescales afterward. Either way the operation counts stay the same, so the circuit may not differ.

I can't remove an operation, but I can try the form `(x + B/A) * A`. That needs a plaintext-plaintext division, which is not allowed, so it is out.

I'll submit a variant that adds the plaintext at a different point, which might alter the circuit's operation order:

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext m = x * a;
  Ciphertext y = m + b;
  y.set_output("y");
}
```