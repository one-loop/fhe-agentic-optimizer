There is still no cheaper circuit to offer. The baseline circuit is one plaintext multiply and one rescale, with no ciphertext multiplies, rotations, adds or alignments. Any change I can make that stays correct either keeps that circuit or adds work.

I'm submitting the minimal kernel unchanged.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext s("s");
  Ciphertext y = x0 * s;
  y.set_output("y");
}
```