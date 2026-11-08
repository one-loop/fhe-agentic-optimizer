I don't think any cheaper circuit exists for this task, so I'm not spending more attempts.

y = x·A + B needs at least one ciphertext × plaintext multiply, because A varies per slot and has to be applied to the ciphertext. It also needs at least one plaintext add, because B can't be folded into the multiply under CKKS. Anything that removes either one would need plaintext-plaintext arithmetic, such as precomputing B/A, which is forbidden. Every other way to write the computation adds operations, such as extra multiplies, adds or alignment steps, so it would be slower.

The rescale after the multiply is inserted by CHEHAB automatically and can't be avoided in the DSL. The baseline is one multiply, one rescale and one add at a single level, which is the minimum.

Here is the optimal kernel, unchanged:

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