I have no circuit cheaper than one plaintext multiply with one rescale, and I've now resubmitted it three times without effect. This time I'm trying a different structure: `y = x0*p` written as `(x0 + x0)*p` minus `x0*p` would add work, so I'm not using that. Instead I'm trying an unusual but legal form, computing the product through a square-free identity that CHEHAB might lower differently.

The identity is x0*p = ((x0+p)^2 - (x0-p)^2)/4. It is invalid here because it mixes a ciphertext with a plaintext in the squares, needs a division by 4 (a constant), and uses ciphertext-ciphertext multiplies. It would be slower and risks failing.

So I can't propose a variant that is both different and plausibly faster. The baseline circuit is the minimum: one ct-pt multiply and one rescale. I'm submitting it again unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```