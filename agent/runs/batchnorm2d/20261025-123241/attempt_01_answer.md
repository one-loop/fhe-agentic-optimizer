The baseline is one ct-pt multiply, one rescale and one plaintext add. The multiply cannot be removed, because A is a per-slot plaintext vector. The plaintext add costs about 2.5 ms for encoding, and the multiply plus rescale costs about 7 ms.

One option is to fold B into the multiply, so that y = (x + B/A) * A. That needs B/A, which is plaintext-plaintext arithmetic and is not allowed. It also still needs a plaintext add, so it saves nothing.

Another option is to avoid the rescale. CHEHAB inserts it automatically, so I can't control it.

The only remaining difference is how the circuit is structured. The plaintext add happens after the rescale, at the lower level, which makes the encoding cheaper. Adding B before the multiply would need B/A, so that isn't available either.

I don't see a real improvement. I'll submit the baseline's structure with the operands written in a different order. CHEHAB likely normalizes this away, so it probably won't count as a new circuit.

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