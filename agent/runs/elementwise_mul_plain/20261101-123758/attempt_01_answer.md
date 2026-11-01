The baseline is already minimal: one ciphertext × plaintext multiply and the rescale CHEHAB inserts after it. The output `y = x0 * p` needs at least one multiplication involving `p`. CKKS forbids constants, so `p` cannot be folded away or precomputed, and the rescale is applied automatically.

Any other circuit would only add work: extra adds, rotations, multiplies or alignments. So I don't expect any of the remaining attempts to beat 7.06 ms, and I'm keeping the baseline. Because its circuit matches the baseline, it cannot be judged a new best. That is expected here, since no strictly cheaper circuit exists.

```cpp
void fhe()
{
  Ciphertext x0("x0");
  Plaintext p("p");
  Ciphertext y = x0 * p;
  y.set_output("y");
}
```