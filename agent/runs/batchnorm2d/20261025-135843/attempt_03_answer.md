The negation variant added ops and was slower, as expected. The baseline already has the minimal ops (1 plaintext multiply, 1 rescale, 1 add), so I need a different way to cut cost.

Idea: the rescale after the multiply is needed to bring the scale back down. Without a ciphertext of ones, B can't be folded into the multiply. A different circuit could be x*a + b written as (x + c)*a, but c would need plaintext arithmetic, which is not allowed.

Another option is to apply the plaintext add to x first, with a plaintext equal to B. That gives (x + b)*a = x*a + a*b, which is not the target function.

A remaining option is to exploit a structural trick: y = x*a + b = (x*a) - (-b). Because `-b` is plaintext arithmetic, it isn't allowed either.

I can't find anything cheaper that is correct. I'll resubmit a variant with a different shape, using a `+=` accumulation. It will probably normalize to the baseline circuit, but it is the only low-risk option left.

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