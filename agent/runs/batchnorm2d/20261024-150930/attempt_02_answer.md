The circuit needs a different structure. The baseline does a plaintext multiply with a rescale, then a plaintext add. The add of B has to be encoded at the product's scale, which costs about 2.5 ms.

One idea is to add B before the multiply, so that the encoding happens at the top level and the add sits on a fresh ciphertext. That would need (x + B/A) * A, which is not allowed because it requires plaintext-plaintext arithmetic.

Another idea is to add B after the rescale. The product is then at a lower level and the plaintext is encoded at the reduced scale, which may avoid some alignment. Written as `x*a` followed by `+ b`, CHEHAB already normalizes this, so the only way to change the circuit is to reorder the operations explicitly. I'll try a form that CHEHAB may handle differently:
- Compute x*a + b as x*a - (-b). This needs plaintext negation, which is plaintext arithmetic and not allowed.
- Use `y = x*a; y += b;`, which is likely the same circuit.

The following variant uses a different structure: splitting the multiply as x*a = x*a, then adding b through a subtraction-free path. It is probably still the same circuit, but it is a valid kernel and tests whether CHEHAB normalizes the ordering.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext y = x * a;
  y += b;
  y.set_output("y");
}
```