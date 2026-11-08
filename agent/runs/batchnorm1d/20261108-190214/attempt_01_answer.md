I don't think this kernel can be made faster. The baseline already uses the minimum possible circuit for y = x·A + B:

- **One ciphertext × plaintext multiply.** This is needed because A varies per slot and no real constants are available except through A and B.
- **One rescale,** which CHEHAB inserts automatically after the multiply.
- **One plaintext add** for B.

There are no rotations, no ciphertext × ciphertext multiplications and no scale alignments, and only one level is consumed.

I ruled out the alternatives:

- **Factoring as (x + B/A)·A** would remove nothing. It would also need plaintext-plaintext arithmetic (B/A), which CKKS forbids.
- **Folding B into the multiply** is impossible. A ciphertext × plaintext product cannot also add an offset.
- **Adding B before multiplying** changes the result unless B is pre-divided by A, which again needs a plaintext-plaintext operation.

Any other circuit would only add operations. So here is the baseline kernel unchanged; it cannot win, because the judge requires a different circuit.

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