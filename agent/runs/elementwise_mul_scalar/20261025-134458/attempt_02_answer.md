Since the plaintext `s` is a scalar, the product x0 * s needs a ct-pt multiply plus a rescale. To get a different circuit, I'll try a structure that might shift where the work happens. The scalar multiply alone is the minimum, so any variation is a gamble. Here, I compute `x0 * s` as `(x0 + x0) * s` halved? That isn't possible without constants, so I try a different decomposition instead: `y = x0*s` written as `(x0*s + x0*s) ` is wrong too.

A valid alternative is to split the product so the multiply happens on two halves of the work: `y = x0*s` is the same function as `x0*s - x0*s + x0*s`, but CHEHAB's simplification would likely collapse that back to the baseline circuit.

I'll try a form that may bypass the collapse: `y = (x0 + x0) * s - x0 * s`. This computes 2·x0·s − x0·s = x0·s. It uses two ct-pt multiplies, which is more expensive unless CHEHAB simplifies it to the baseline. It is a different structure, but likely not faster. It is a low-risk probe of what the simplifier does.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext d = x0 + x0;
  Ciphertext y = d * s - x0 * s;
  y.set_output("y");
}
```