The baseline needs one plaintext multiply (with a rescale) and one plaintext add. The add costs about 2.5 ms because the plaintext B has to be encoded.

Here is an idea that changes the circuit: compute y = (x + B*A⁻¹) * A. That needs a plaintext-plaintext operation, which is not allowed. The alternative is to put the add before the multiply with a different plaintext, but any plaintext derived from A and B is plaintext arithmetic, so this is also blocked.

What remains is to change the evaluation order so the plaintext add happens before the rescale. The DSL decides this, so I can't control it directly. I will try a form that CHEHAB may handle differently: y = x*A + B written as x*A - (0 - B). Subtraction from a plaintext needs a ciphertext operand, so it cannot be built without a constant. Next I'll try `-(-(x*a) - b)`, which negates the product and then subtracts b. This adds negations (extra ops), so it is likely slower, but it does change the circuit, and the negation of a ciphertext is cheap.

I don't expect this to be faster. It does give a different circuit to test.

```cpp
void fhe()
{
  Ciphertext x("x");
  Plaintext a("A");
  Plaintext b("B");
  Ciphertext t = -(x * a);
  Ciphertext y = -(t - b);
  y.set_output("y");
}
```