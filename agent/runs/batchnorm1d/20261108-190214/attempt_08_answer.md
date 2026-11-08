Attempt 7 again produced the same circuit as the baseline, so no improvement was found. All seven attempts so far returned the baseline unchanged, and I don't expect a faster circuit to exist.

The baseline already does only what y = x·A + B requires:
- **One multiply by A.** A is different in each slot, so it has to be applied to the ciphertext.
- **One add of B.** A multiply can't also add an offset.
- **One rescale,** which CHEHAB inserts automatically after the multiply.

Any different circuit would have to add an operation, and so would be slower. Removing an operation would need plaintext-plaintext arithmetic such as precomputing B/A, which this setup forbids.

The last attempt can't produce a winner either, so the baseline is the final answer at 3.79 ms:

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